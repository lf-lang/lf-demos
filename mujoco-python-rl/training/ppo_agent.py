from __future__ import annotations

from dataclasses import dataclass

import numpy as np
import torch
from torch import nn
from torch.distributions import Normal

# this class holds the config for PPO, ActorCritic Network, training
@dataclass
class PPOConfig:
    observation_dim: int
    action_dim: int
    hidden_sizes: tuple[int, ...] = (128, 128)
    learning_rate: float = 3e-4
    gamma: float = 0.99
    gae_lambda: float = 0.95
    clip_coef: float = 0.2
    entropy_coef: float = 0.01
    entropy_coef_final: float = 0.01
    value_coef: float = 0.5
    max_grad_norm: float = 0.5
    update_epochs: int = 5
    minibatch_size: int = 256

# this class defines the Actor Critic model that PPO uses for learning the gain scheduling policy
class ActorCritic(nn.Module):
    def __init__(self, observation_dim: int, action_dim: int, hidden_sizes: tuple[int, ...]):
        super().__init__()
        layers: list[nn.Module] = []
        last_dim = observation_dim
        for hidden in hidden_sizes:
            layers += [nn.Linear(last_dim, hidden), nn.Tanh()]
            last_dim = hidden
        self.backbone = nn.Sequential(*layers)
        self.actor_mean = nn.Linear(last_dim, action_dim)
        self.critic = nn.Linear(last_dim, 1)
        self.log_std = nn.Parameter(torch.full((action_dim,), -0.5))

    def forward(self, obs: torch.Tensor) -> tuple[torch.Tensor, torch.Tensor, torch.Tensor]:
        features = self.backbone(obs)
        mean = torch.tanh(self.actor_mean(features))
        value = self.critic(features).squeeze(-1)
        return mean, self.log_std.expand_as(mean), value

    def distribution(self, obs: torch.Tensor) -> tuple[Normal, torch.Tensor]:
        mean, log_std, value = self(obs)
        return Normal(mean, log_std.exp()), value

    # action produced from observation
    def act(self, obs: np.ndarray, deterministic: bool = False) -> tuple[np.ndarray, float, float]:
        obs_t = torch.as_tensor(obs, dtype=torch.float32).unsqueeze(0)
        with torch.no_grad():
            dist, value = self.distribution(obs_t)
            action_t = dist.mean if deterministic else dist.sample()
            action_t = torch.clamp(action_t, -1.0, 1.0)
            logprob_t = dist.log_prob(action_t).sum(-1)
        action = action_t.squeeze(0).cpu().numpy()
        return action, float(logprob_t.item()), float(value.item())

# stores chunk of experience under current policy
# this data structure holds those interactions
class RolloutBuffer:
    def __init__(self, size: int, observation_dim: int, action_dim: int):
        self.size = size
        self.observations = np.zeros((size, observation_dim), dtype=np.float32)
        self.actions = np.zeros((size, action_dim), dtype=np.float32)
        self.logprobs = np.zeros(size, dtype=np.float32)
        self.rewards = np.zeros(size, dtype=np.float32)
        self.dones = np.zeros(size, dtype=np.float32)
        self.values = np.zeros(size, dtype=np.float32)
        self.advantages = np.zeros(size, dtype=np.float32)
        self.returns = np.zeros(size, dtype=np.float32)
        self.ptr = 0

    # append new experience to rollout buffer
    def add(self, obs: np.ndarray, action: np.ndarray, logprob: float, reward: float, done: bool, value: float) -> None:
        if self.ptr >= self.size:
            raise IndexError("rollout buffer is full")
        self.observations[self.ptr] = obs
        self.actions[self.ptr] = action
        self.logprobs[self.ptr] = logprob
        self.rewards[self.ptr] = reward
        self.dones[self.ptr] = float(done)
        self.values[self.ptr] = value
        self.ptr += 1

    # generalized advantage estimation
    def compute_returns(self, last_value: float, gamma: float, gae_lambda: float) -> None:
        advantage = 0.0
        for t in reversed(range(self.ptr)):
            next_nonterminal = 1.0 - self.dones[t]
            next_value = last_value if t == self.ptr - 1 else self.values[t + 1]
            delta = self.rewards[t] + gamma * next_value * next_nonterminal - self.values[t]
            advantage = delta + gamma * gae_lambda * next_nonterminal * advantage
            self.advantages[t] = advantage
        self.returns[: self.ptr] = self.advantages[: self.ptr] + self.values[: self.ptr]

    # shuffles minibatches for training 
    def batches(self, minibatch_size: int):
        indices = np.arange(self.ptr)
        np.random.shuffle(indices)
        for start in range(0, self.ptr, minibatch_size):
            yield indices[start : start + minibatch_size]

# this class owns network and optimizer
class PPOTrainer:
    def __init__(self, config: PPOConfig):
        self.config = config
        self.model = ActorCritic(config.observation_dim, config.action_dim, config.hidden_sizes)
        self.optimizer = torch.optim.Adam(self.model.parameters(), lr=config.learning_rate)

    # ppo training function
    def update(self, buffer: RolloutBuffer, entropy_coef: float | None = None) -> dict[str, float]:
        # converts rollout buffer to PyTorch tensors
        obs = torch.as_tensor(buffer.observations[: buffer.ptr], dtype=torch.float32)
        actions = torch.as_tensor(buffer.actions[: buffer.ptr], dtype=torch.float32)
        old_logprobs = torch.as_tensor(buffer.logprobs[: buffer.ptr], dtype=torch.float32)
        advantages = torch.as_tensor(buffer.advantages[: buffer.ptr], dtype=torch.float32)
        returns = torch.as_tensor(buffer.returns[: buffer.ptr], dtype=torch.float32)
        advantages = (advantages - advantages.mean()) / (advantages.std() + 1e-8)
        entropy_coef = self.config.entropy_coef if entropy_coef is None else float(entropy_coef)

        last_policy_loss = 0.0
        last_value_loss = 0.0
        last_entropy = 0.0

        # main update loop for ppo
        for _ in range(self.config.update_epochs):
            for batch in buffer.batches(self.config.minibatch_size):
                # compute policy distrobution
                dist, values = self.model.distribution(obs[batch])
                new_logprobs = dist.log_prob(actions[batch]).sum(-1)
                entropy = dist.entropy().sum(-1).mean()
                ratio = (new_logprobs - old_logprobs[batch]).exp() # compute probability ratio

                # compute Loss using clipped objective (PPO)
                unclipped = ratio * advantages[batch]
                clipped = torch.clamp(ratio, 1.0 - self.config.clip_coef, 1.0 + self.config.clip_coef) * advantages[batch]
                policy_loss = -torch.min(unclipped, clipped).mean()
                value_loss = 0.5 * (returns[batch] - values).pow(2).mean() # trains critic
                loss = policy_loss + self.config.value_coef * value_loss - entropy_coef * entropy # compute total loss including entropy

                # gradient update
                self.optimizer.zero_grad()
                loss.backward()
                nn.utils.clip_grad_norm_(self.model.parameters(), self.config.max_grad_norm)
                self.optimizer.step()

                last_policy_loss = float(policy_loss.item())
                last_value_loss = float(value_loss.item())
                last_entropy = float(entropy.item())

        return {
            "policy_loss": last_policy_loss,
            "value_loss": last_value_loss,
            "entropy": last_entropy,
            "entropy_coef": entropy_coef,
        }
