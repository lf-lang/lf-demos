from __future__ import annotations

from dataclasses import dataclass
from typing import Any

import numpy as np

from src.controllers.pid import PID
from src.kinematics.ik import InverseKinematics
from src.mujoco_env.env import MujocoEnv
from src.tasks.target_gen import TargetGenerator, Task
from src.types.observation import Observation

# this class stores config variables related to the training env
@dataclass
class GainScheduleEnvConfig:
    model_path: str = "src/mujoco_env/3dof_leg.xml"
    episode_steps: int = 2000
    policy_interval_steps: int = 5
    dt: float = 0.002
    planning_dt: float = 0.01
    random_gravity: bool = True
    random_tilt: bool = True
    vary_model: bool = True
    gravity_mag_range: tuple[float, float] = (0.0, 15.0)
    tilt_xy_range: tuple[float, float] = (-0.5, 0.5)
    model_variation_frac: float = 0.1
    disturbance_count_range: tuple[int, int] = (0, 10)
    noise_std: float = 0.0
    delay_steps: int = 0
    task_names: tuple[str, ...] = ("trajectory",)
    task_probs: tuple[float, ...] | None = None
    use_grav_comp: bool = False
    use_inv_dyn: bool = False
    use_weighted_error: bool = False
    kp_min: float = 0.05
    kp_max: float = 80.0
    ki_min: float = 0.0
    ki_max: float = 10.0
    kd_min: float = 0.0
    kd_max: float = 8.0
    kaw_min: float = 0.0
    kaw_max: float = 2.0
    action_gain_smoothing: float = 0.35
    tracking_weight: float = 20.0
    joint_tracking_weight: float = 1.0
    velocity_weight: float = 0.02
    torque_weight: float = 0.02
    torque_rate_weight: float = 0.0
    joint_accel_weight: float = 0.0
    saturation_weight: float = 0.2
    gain_magnitude_weight: float = 0.0
    gain_rate_weight: float = 0.005
    failure_penalty: float = 100.0
    reward_scale: float = 0.01

# this class was created separate of what is used in LF so that trianing the actor critic network is easier
class GainScheduleEnv:

    action_dim = 12 # the 4 control gains per leg
    observation_dim = 36 # the tracking error metrics

    def __init__(self, config: GainScheduleEnvConfig | None = None):
        self.config = config or GainScheduleEnvConfig()
        self.env: MujocoEnv | None = None
        self.pid: PID | None = None
        self.ik: InverseKinematics | None = None
        self.tgen: TargetGenerator | None = None
        self.rng = np.random.default_rng(0)
        self.obs_noise_rng = np.random.default_rng(1000003)
        self.delay_queue: list[Observation] = []
        self.current_obs: Observation | None = None
        self.last_target = None
        self.current_task = Task.TRAJECTORY
        self.last_joint_pos = np.zeros(3)
        self.last_joint_vel = np.zeros(3)
        self.last_joint_acc = np.zeros(3)
        self.last_used_ik_fallback = False
        self.last_task_error = np.zeros(3)
        self.last_joint_error = np.zeros(3)
        self.last_torque = np.zeros(3)
        self.last_torque_delta = np.zeros(3)
        self.last_joint_accel_est = np.zeros(3)
        self.last_action = np.zeros(self.action_dim)
        self.last_used_ik_fallback = False
        self.current_gains = self.default_gains()
        self.steps = 0
        self.plan_steps = max(1, round(self.config.planning_dt / self.config.dt))
        self.target_time = 0.0

    # resets training env; used at the start of every episode; wraps the already-existing reset function
    def reset(self, seed: int | None = None) -> tuple[np.ndarray, dict[str, Any]]:
        if seed is None:
            seed = int(self.rng.integers(0, 2**31 - 1))
        self.rng = np.random.default_rng(seed)
        self.obs_noise_rng = np.random.default_rng(seed + 1000003)
        self.env = MujocoEnv(self.config.model_path, self.config.dt)
        self.env.gravity_mag_range = self.config.gravity_mag_range
        self.env.tilt_xy_range = self.config.tilt_xy_range
        self.env.model_variation_frac = self.config.model_variation_frac
        self.env.disturbance_count_range = self.config.disturbance_count_range
        self.current_obs = self.env.reset(
            seed=seed,
            random_gravity=self.config.random_gravity,
            random_tilt=self.config.random_tilt,
            vary_model=self.config.vary_model,
            episode_dur=self.config.episode_steps,
            noise_std=self.config.noise_std,
        )
        self.current_obs = self.observe(self.current_obs)
        self.pid = PID(3, self.env)
        self.ik = InverseKinematics(self.env)
        self.tgen = TargetGenerator(self.env)
        self.current_task = self.sample_task()
        self.tgen.set_task(self.current_task)
        self.steps = 0
        self.target_time = 0.0
        self.delay_queue = []
        self.last_torque = np.zeros(3)
        self.last_torque_delta = np.zeros(3)
        self.last_joint_accel_est = np.zeros(3)
        self.last_action = np.zeros(self.action_dim)
        self.current_gains = self.default_gains()
        self.set_pid_params(self.current_gains)
        ok = self.update_target()
        obs = self.policy_observation()
        info = self.info(ik_converged=ok)
        return obs, info

    # step the env
    def step(self, action: np.ndarray) -> tuple[np.ndarray, float, bool, bool, dict[str, Any]]:
        assert self.env is not None
        assert self.pid is not None
        action = np.asarray(action, dtype=np.float64)
        if action.shape != (self.action_dim,):
            raise ValueError(f"action must have shape {(self.action_dim,)}, got {action.shape}")

        # apply new gains during this step
        prev_gains = self.current_gains.copy()
        requested_gains = self.action_to_gains(action)
        self.current_gains = (
            (1.0 - self.config.action_gain_smoothing) * self.current_gains
            + self.config.action_gain_smoothing * requested_gains
        )
        self.set_pid_params(self.current_gains)
        self.last_action = action.copy()

        reward = 0.0
        terminated = False
        ik_converged = True

        for _ in range(self.config.policy_interval_steps):
            if self.steps % self.plan_steps == 0:
                ik_converged = self.update_target()
                if not ik_converged:
                    terminated = True

            # get metrics from previous step
            # compute control effort from new gains
            prev_torque = self.last_torque.copy()
            prev_qd = self.current_obs.qd.copy()
            command = self.pid.compute_effort(
                True,
                self.config.use_grav_comp,
                self.config.use_inv_dyn,
                self.config.use_weighted_error,
                self.last_joint_pos,
                self.current_obs.q,
                self.current_obs.qd,
                self.last_joint_vel,
                self.last_joint_acc,
            )
            self.last_torque = command.torques
            self.last_torque_delta = self.last_torque - prev_torque
            
            # step env with computed effort
            raw_obs = self.env.step(command.torques)
            
            # add noise and delay to resulting obs
            self.current_obs = self.observe(raw_obs)
            self.last_joint_accel_est = (self.current_obs.qd - prev_qd) / self.config.dt
            self.update_errors()
            
            # accumulate reward
            reward += self.reward(prev_gains)
            self.steps += 1

            if self.steps >= self.config.episode_steps:
                break
            if not np.isfinite(self.policy_observation()).all():
                terminated = True
            if terminated:
                reward -= self.config.failure_penalty
                break

        truncated = self.steps >= self.config.episode_steps
        reward *= self.config.reward_scale
        return self.policy_observation(), float(reward), terminated, truncated, self.info(ik_converged)

    # sample a random action to be executed
    def sample_action(self) -> np.ndarray:
        return self.rng.uniform(-1.0, 1.0, size=self.action_dim)

    # sample a task for the plant to execute during a training episode
    def sample_task(self) -> Task:
        task_map = {
            "pose": Task.POSE,
            "sinusoid": Task.SINUSOID,
            "trajectory": Task.TRAJECTORY,
        }
        names = tuple(name.lower() for name in self.config.task_names)
        if not names:
            raise ValueError("task_names must not be empty")
        probs = None
        if self.config.task_probs is not None:
            probs = np.asarray(self.config.task_probs, dtype=np.float64)
            if probs.shape != (len(names),):
                raise ValueError("task_probs must match task_names length")
            probs = probs / probs.sum()
        selected = self.rng.choice(names, p=probs)
        return task_map[selected]

    # helper function for getting the starting configuration
    def default_gains(self) -> np.ndarray:
        return np.array([1.0, 1.0, 1.0, 0.1, 0.1, 0.1, 0.1, 0.1, 0.1, 0.1, 0.1, 0.1], dtype=np.float64)

    # actions are decoded to usable gains
    def action_to_gains(self, action: np.ndarray) -> np.ndarray:
        ranges = np.array(
            [
                [self.config.kp_min, self.config.kp_max],
                [self.config.kp_min, self.config.kp_max],
                [self.config.kp_min, self.config.kp_max],
                [self.config.ki_min, self.config.ki_max],
                [self.config.ki_min, self.config.ki_max],
                [self.config.ki_min, self.config.ki_max],
                [self.config.kd_min, self.config.kd_max],
                [self.config.kd_min, self.config.kd_max],
                [self.config.kd_min, self.config.kd_max],
                [self.config.kaw_min, self.config.kaw_max],
                [self.config.kaw_min, self.config.kaw_max],
                [self.config.kaw_min, self.config.kaw_max],
            ],
            dtype=np.float64,
        )
        normalized = np.clip(action, -1.0, 1.0)
        unit = 0.5 * (normalized + 1.0)
        return ranges[:, 0] + unit * (ranges[:, 1] - ranges[:, 0])

    # helper function for setting the control params
    def set_pid_params(self, gains: np.ndarray) -> None:
        self.pid.set_params(
            kp=gains[0:3],
            ki=gains[3:6],
            kd=gains[6:9],
            dt=self.config.dt,
            beta=np.zeros(3),
            alpha=np.full(3, 0.9),
            u_min=np.full(3, -1.0),
            u_max=np.full(3, 1.0),
            kaw=gains[9:12],
        )

    # hlper function for updating the task and joint targets the plant needs to achieve
    def update_target(self) -> bool:
        assert self.tgen is not None
        assert self.ik is not None
        self.last_target = self.tgen.compute_target(self.target_time)
        self.target_time += self.config.planning_dt
        joint_pos, converged = self.ik.compute_joint_pos(self.last_target.task_pos, self.last_target.ik_seed)
        self.last_used_ik_fallback = False
        if not converged and self.last_target.ik_seed is not None:
            joint_pos = np.clip(self.last_target.ik_seed.copy(), self.env.joint_min, self.env.joint_max)
            self.ik.q = joint_pos.copy()
            converged = True
            self.last_used_ik_fallback = True
        joint_vel, joint_acc = self.ik.compute_joint_vel(self.last_target.task_vel)
        self.last_joint_pos = joint_pos
        self.last_joint_vel = joint_vel
        self.last_joint_acc = joint_acc
        self.update_errors()
        return bool(converged)

    # introduce observation noise and delays in receiving observations during training
    # simulates imperfect plant
    # makes control policy more robust
    def observe(self, obs: Observation) -> Observation:
        noisy = obs
        std = float(self.config.noise_std)
        if std > 0.0:
            noisy = Observation(
                q=obs.q + self.obs_noise_rng.normal(0.0, std, size=obs.q.shape),
                qd=obs.qd + self.obs_noise_rng.normal(0.0, std, size=obs.qd.shape),
                ee_pos=obs.ee_pos + self.obs_noise_rng.normal(0.0, std, size=obs.ee_pos.shape),
                ee_lin_vel=obs.ee_lin_vel + self.obs_noise_rng.normal(0.0, std, size=obs.ee_lin_vel.shape),
                ee_ang_vel=obs.ee_ang_vel + self.obs_noise_rng.normal(0.0, std, size=obs.ee_ang_vel.shape),
                masses=obs.masses,
                inertias=obs.inertias,
                damping=obs.damping,
                gravity=obs.gravity,
                seed=obs.seed,
                noise_std=std,
            )
        delay = int(self.config.delay_steps)
        if delay <= 0:
            return noisy
        if not self.delay_queue:
            self.delay_queue = [noisy] * (delay + 1)
        delayed = self.delay_queue.pop(0)
        self.delay_queue.append(noisy)
        return delayed

    # helper function for updating task and joint error states
    def update_errors(self) -> None:
        self.last_joint_error = self.last_joint_pos - self.current_obs.q
        if self.last_target is not None and self.current_obs.ee_pos is not None:
            self.last_task_error = self.last_target.task_pos - self.current_obs.ee_pos

    # get the observation vector that the policy is trained on; used to get action according to policy
    def policy_observation(self) -> np.ndarray:
        task_vel = np.zeros(3) if self.last_target is None else self.last_target.task_vel
        obs = np.concatenate(
            [
                self.current_obs.q,
                self.current_obs.qd,
                self.last_joint_pos,
                self.last_joint_vel,
                self.last_joint_error,
                self.last_task_error,
                task_vel,
                self.last_torque,
                self.current_gains / np.array([80.0] * 3 + [10.0] * 3 + [8.0] * 3 + [2.0] * 3),
            ]
        )
        return obs.astype(np.float32)

    # computes reward for making the action (gain schedule) at the current state (env observation)
    def reward(self, prev_gains: np.ndarray) -> float:
        # penalize task and joint error
        task_cost = self.config.tracking_weight * float(self.last_task_error @ self.last_task_error)
        joint_cost = self.config.joint_tracking_weight * float(self.last_joint_error @ self.last_joint_error)
        # penalize velocity
        vel_cost = self.config.velocity_weight * float(self.current_obs.qd @ self.current_obs.qd)
        # penalize torque and changes in torque
        torque_cost = self.config.torque_weight * float(self.last_torque @ self.last_torque)
        torque_rate_cost = self.config.torque_rate_weight * float(self.last_torque_delta @ self.last_torque_delta)
        # penalize joint acceleration
        joint_accel_cost = self.config.joint_accel_weight * float(self.last_joint_accel_est @ self.last_joint_accel_est)
        # penalize saturating the actuators
        saturation = np.maximum(np.abs(self.last_torque) - 0.98, 0.0)
        saturation_cost = self.config.saturation_weight * float(saturation @ saturation)
        # normalize gains
        gain_scale = np.array([self.config.kp_max] * 3 + [self.config.ki_max] * 3 + [self.config.kd_max] * 3 + [self.config.kaw_max] * 3)
        normalized_gains = self.current_gains / gain_scale
        # penalize large gains and large gain jumps
        gain_magnitude_cost = self.config.gain_magnitude_weight * float(normalized_gains @ normalized_gains)
        gain_rate = (self.current_gains - prev_gains) / np.array([80.0] * 3 + [10.0] * 3 + [8.0] * 3 + [2.0] * 3)
        gain_rate_cost = self.config.gain_rate_weight * float(gain_rate @ gain_rate)
        return -(
            task_cost
            + joint_cost
            + vel_cost
            + torque_cost
            + torque_rate_cost
            + joint_accel_cost
            + saturation_cost
            + gain_magnitude_cost
            + gain_rate_cost
        )

    # gives dict of info related to the env state and plant metrics
    def info(self, ik_converged: bool) -> dict[str, Any]:
        assert self.env is not None
        return {
            "step": self.steps,
            "ik_converged": ik_converged,
            "ik_fallback": self.last_used_ik_fallback,
            "task": self.current_task.name.lower(),
            "task_error_norm": float(np.linalg.norm(self.last_task_error)),
            "joint_error_norm": float(np.linalg.norm(self.last_joint_error)),
            "gains": self.current_gains.copy(),
            "torque": self.last_torque.copy(),
            "hidden_gravity": self.env.gravity.copy(),
        }
