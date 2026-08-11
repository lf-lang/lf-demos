from __future__ import annotations

import argparse
import csv
import json
from pathlib import Path

import numpy as np
import torch

from src.rl.gain_schedule_env import GainScheduleEnv, GainScheduleEnvConfig
from training.ppo_agent import PPOConfig, PPOTrainer, RolloutBuffer

# helper for loading training configs
def load_config(path: Path) -> dict:
    with path.open("r", encoding="utf-8") as f:
        return json.load(f)


def linear_schedule(start: float, end: float, progress: float) -> float:
    progress = min(max(progress, 0.0), 1.0)
    return start + progress * (end - start)


def main() -> None:
    parser = argparse.ArgumentParser()
    parser.add_argument("--config", default="configs/ppo_gain_schedule.json")
    args = parser.parse_args()

    raw = load_config(Path(args.config))
    seed = int(raw.get("seed", 0))
    np.random.seed(seed)
    torch.manual_seed(seed)

    # load config for env
    env_config = GainScheduleEnvConfig(
        episode_steps=int(raw["episode_steps"]),
        policy_interval_steps=int(raw["policy_interval_steps"]),
        noise_std=float(raw.get("noise_std", 0.0)),
        delay_steps=int(raw.get("delay_steps", 0)),
        task_names=tuple(raw.get("task_names", ["trajectory"])),
        task_probs=tuple(raw["task_probs"]) if "task_probs" in raw else None,
        random_gravity=bool(raw.get("random_gravity", True)),
        random_tilt=bool(raw.get("random_tilt", True)),
        vary_model=bool(raw.get("vary_model", True)),
        gravity_mag_range=tuple(raw.get("gravity_mag_range", [7.0, 13.0])),
        tilt_xy_range=tuple(raw.get("tilt_xy_range", [-0.5, 0.5])),
        model_variation_frac=float(raw.get("model_variation_frac", 0.1)),
        disturbance_count_range=tuple(raw.get("disturbance_count_range", [0, 10])),
        kp_max=float(raw.get("kp_max", GainScheduleEnvConfig.kp_max)),
        ki_max=float(raw.get("ki_max", GainScheduleEnvConfig.ki_max)),
        kd_max=float(raw.get("kd_max", GainScheduleEnvConfig.kd_max)),
        kaw_max=float(raw.get("kaw_max", GainScheduleEnvConfig.kaw_max)),
        action_gain_smoothing=float(
            raw.get("action_gain_smoothing", GainScheduleEnvConfig.action_gain_smoothing)
        ),
        tracking_weight=float(raw.get("tracking_weight", GainScheduleEnvConfig.tracking_weight)),
        joint_tracking_weight=float(raw.get("joint_tracking_weight", GainScheduleEnvConfig.joint_tracking_weight)),
        torque_weight=float(raw.get("torque_weight", GainScheduleEnvConfig.torque_weight)),
        torque_rate_weight=float(raw.get("torque_rate_weight", GainScheduleEnvConfig.torque_rate_weight)),
        joint_accel_weight=float(raw.get("joint_accel_weight", GainScheduleEnvConfig.joint_accel_weight)),
        saturation_weight=float(raw.get("saturation_weight", GainScheduleEnvConfig.saturation_weight)),
        gain_magnitude_weight=float(raw.get("gain_magnitude_weight", GainScheduleEnvConfig.gain_magnitude_weight)),
        gain_rate_weight=float(raw.get("gain_rate_weight", GainScheduleEnvConfig.gain_rate_weight)),
        reward_scale=float(raw.get("reward_scale", GainScheduleEnvConfig.reward_scale)),
    )
    # create env
    env = GainScheduleEnv(env_config)
    # load ppo config
    ppo_config = PPOConfig(
        observation_dim=env.observation_dim,
        action_dim=env.action_dim,
        hidden_sizes=tuple(raw.get("hidden_sizes", [128, 128])),
        learning_rate=float(raw["learning_rate"]),
        gamma=float(raw["gamma"]),
        gae_lambda=float(raw["gae_lambda"]),
        clip_coef=float(raw["clip_coef"]),
        entropy_coef=float(raw["entropy_coef"]),
        entropy_coef_final=float(raw.get("entropy_coef_final", raw["entropy_coef"])),
        value_coef=float(raw["value_coef"]),
        max_grad_norm=float(raw["max_grad_norm"]),
        update_epochs=int(raw["update_epochs"]),
        minibatch_size=int(raw["minibatch_size"]),
    )
    # create ppo trainer
    trainer = PPOTrainer(ppo_config)

    # init env and logging
    obs, _ = env.reset(seed=seed)
    total_timesteps = int(raw["total_timesteps"])
    rollout_steps = int(raw["rollout_steps"])
    global_step = 0
    episode_return = 0.0
    episode_index = 0
    update_index = 0

    episode_log_path = Path(raw.get("episode_log_path", "training/logs/ppo_episodes.csv"))
    update_log_path = Path(raw.get("update_log_path", "training/logs/ppo_updates.csv"))
    episode_log_path.parent.mkdir(parents=True, exist_ok=True)
    update_log_path.parent.mkdir(parents=True, exist_ok=True)

    episode_file = episode_log_path.open("w", newline="", encoding="utf-8")
    update_file = update_log_path.open("w", newline="", encoding="utf-8")
    episode_writer = csv.writer(episode_file)
    update_writer = csv.writer(update_file)
    episode_writer.writerow(
        [
            "episode",
            "global_step",
            "return",
            "task_error_norm",
            "joint_error_norm",
            "terminated",
            "truncated",
            "ik_converged",
            "ik_fallback",
            "task",
        ]
    )
    update_writer.writerow(["update", "global_step", "policy_loss", "value_loss", "entropy", "entropy_coef"])

    try:
        while global_step < total_timesteps:
            buffer = RolloutBuffer(rollout_steps, env.observation_dim, env.action_dim)
            while buffer.ptr < rollout_steps and global_step < total_timesteps:
                
                # act in env and accumulate rewards under current policy
                action, logprob, value = trainer.model.act(obs)
                next_obs, reward, terminated, truncated, info = env.step(action)
                done = terminated or truncated
                buffer.add(obs, action, logprob, reward, done, value)
                episode_return += reward
                obs = next_obs
                global_step += 1

                if done:
                    print(
                        f"episode={episode_index} global_step={global_step} "
                        f"return={episode_return:.3f} task_error={info['task_error_norm']:.5f} "
                        f"terminated={terminated} truncated={truncated} ik={info['ik_converged']}"
                    )
                    episode_writer.writerow(
                        [
                            episode_index,
                            global_step,
                            episode_return,
                            info["task_error_norm"],
                            info["joint_error_norm"],
                            terminated,
                            truncated,
                            info["ik_converged"],
                            info["ik_fallback"],
                            info["task"],
                        ]
                    )
                    episode_file.flush()
                    episode_index += 1
                    episode_return = 0.0
                    obs, _ = env.reset(seed=seed + episode_index)

            with torch.no_grad():
                _, _, last_value = trainer.model.act(obs, deterministic=True)
            buffer.compute_returns(last_value, ppo_config.gamma, ppo_config.gae_lambda)
            # eval
            progress = global_step / total_timesteps
            entropy_coef = linear_schedule(ppo_config.entropy_coef, ppo_config.entropy_coef_final, progress)
            # update ppo policy
            metrics = trainer.update(buffer, entropy_coef=entropy_coef)
            print(
                f"update global_step={global_step} "
                f"policy_loss={metrics['policy_loss']:.4f} "
                f"value_loss={metrics['value_loss']:.4f} "
                f"entropy={metrics['entropy']:.4f} "
                f"entropy_coef={metrics['entropy_coef']:.5f}"
            )
            update_writer.writerow(
                [
                    update_index,
                    global_step,
                    metrics["policy_loss"],
                    metrics["value_loss"],
                    metrics["entropy"],
                    metrics["entropy_coef"],
                ]
            )
            update_file.flush()
            update_index += 1
    finally:
        episode_file.close()
        update_file.close()

    # save learned policy
    checkpoint_path = Path(raw["checkpoint_path"])
    checkpoint_path.parent.mkdir(parents=True, exist_ok=True)
    torch.save(
        {
            "model_state_dict": trainer.model.state_dict(),
            "config": raw,
        },
        checkpoint_path,
    )
    print(f"saved checkpoint: {checkpoint_path}")


if __name__ == "__main__":
    main()
