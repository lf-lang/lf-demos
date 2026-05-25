import numpy as np
from ..mujoco_env.env import MujocoEnv
from ..types.target import Target
from enum import Enum
import mujoco

# enum class for defining the tasks the plant needs to execute
class Task(Enum):
    POSE = 1
    SINUSOID = 2
    TRAJECTORY = 3

# class used for generating targets
class TargetGenerator:
    def __init__(self, env: MujocoEnv):
        self.env = env
        self.task = 0

        self.pose_task_pos = np.zeros(3)
        self.pose_task_vel = np.zeros(3)

        self.buffer = mujoco.MjData(self.env.model)
        self.A = 0.0
        self.w = 0
        self.sin_q_center = np.zeros(3)
        self.sin_q_amp = np.zeros(3)
        self.sin_q_phase = np.zeros(3)

        self.x0 = np.zeros(3)
        self.xf = np.zeros(3)
        self.q0 = np.zeros(3)
        self.qf = np.zeros(3)
        self.q_waypoints = []
        self.x_waypoints = []
        self.segment_duration = 1.0

    # sets the task for the plant to execute
    def set_task(self, desired_task: Task):
        self.task = desired_task

        # computes random position to go to using mujoco
        if (desired_task == Task.POSE):
            self.buffer.qvel[:] = 0.0
            q1 = self.env.compute_random_position()
            self.buffer.qpos[:] = q1
            mujoco.mj_forward(self.env.model, self.buffer)
            
            self.q0 = q1.copy()
            self.pose_task_pos = self.buffer.site_xpos[self.env.ee_site_id].copy()
            self.pose_task_vel = np.array([0.0, 0.0, 0.0])
        # defines parameters of the sinusoidal joint trajectory
        elif (desired_task == Task.SINUSOID):
            self.sin_q_center = self.env.init_pose.copy()
            self.sin_q_amp = np.minimum(
                np.array([0.18, 0.35, 0.35]),
                0.45 * np.minimum(self.sin_q_center - self.env.joint_min, self.env.joint_max - self.sin_q_center),
            )
            self.sin_q_amp = np.maximum(self.sin_q_amp, np.array([0.04, 0.08, 0.08]))
            self.sin_q_phase = self.env.rng.uniform(0.0, 2.0 * np.pi, size=3)
            self.w = self.env.rng.uniform(1.0, 3.0)
        # defines waypoints for end-effector to go through along the desired trajectory
        elif (desired_task == Task.TRAJECTORY):
            self.q0 = self.env.init_pose.copy()
            self.q_waypoints = [self.q0]
            for _ in range(4):
                self.q_waypoints.append(self.sample_nearby_joint_target(self.q_waypoints[-1]))
            self.q_waypoints.append(self.q0.copy())
            self.x_waypoints = [self.fk(q) for q in self.q_waypoints]
            self.x0 = self.x_waypoints[0]
            self.xf = self.x_waypoints[1]

    # compute the target the end-effector should be at at time t
    def compute_target(self, t):
        if (self.task == Task.POSE): # this is static for all t
            return Target(task_pos=self.pose_task_pos, task_vel=self.pose_task_vel, ik_seed=self.q0)
        elif (self.task == Task.SINUSOID):
            # make sure params of sinusoid are physically realizable
            # compute desired joint pos and vel at time t
            q_seed = self.sin_q_center + self.sin_q_amp * np.sin(self.w * t + self.sin_q_phase)
            q_seed = np.clip(q_seed, self.env.joint_min, self.env.joint_max)
            qd_seed = self.sin_q_amp * self.w * np.cos(self.w * t + self.sin_q_phase)

            # use mujoco to compute enf-effector pos and vel at time t
            jacp = np.zeros((3, self.env.model.nv))
            jacr = np.zeros((3, self.env.model.nv))
            self.buffer.qpos[:] = q_seed
            self.buffer.qvel[:] = qd_seed
            mujoco.mj_forward(self.env.model, self.buffer)
            mujoco.mj_jacSite(self.env.model, self.buffer, jacp, jacr, self.env.ee_site_id)
            pos = self.buffer.site_xpos[self.env.ee_site_id].copy()
            vel = jacp @ qd_seed
            return Target(task_pos=pos, task_vel=vel, ik_seed=q_seed)
        elif (self.task == Task.TRAJECTORY):
            # compute segments of trajectory and their cycle time
            num_segments = max(len(self.x_waypoints) - 1, 1)
            cycle_time = num_segments * self.segment_duration
            t_cycle = t % cycle_time
            segment = min(int(t_cycle / self.segment_duration), num_segments - 1)
            tau = (t_cycle - segment * self.segment_duration) / self.segment_duration
            s = 3 * tau**2 - 2 * tau**3
            sd = (6 * tau - 6 * tau**2) / self.segment_duration
            
            # compute waypoints
            x0 = self.x_waypoints[segment]
            x1 = self.x_waypoints[segment+1]
            q0 = self.q_waypoints[segment]
            q1 = self.q_waypoints[segment+1]
            x = x0 + (x1 - x0) * s
            xd = (x1 - x0) * sd
            q_seed = q0 + (q1 - q0) * s
            return Target(task_pos=x, task_vel=xd, ik_seed=q_seed)

    # fk helper for choosing physically realizable trajectories
    def fk(self, q: np.ndarray) -> np.ndarray:
        self.buffer.qpos[:] = q
        self.buffer.qvel[:] = 0.0
        mujoco.mj_forward(self.env.model, self.buffer)
        return self.buffer.site_xpos[self.env.ee_site_id].copy()

    # sampling helper for choosing physically realizable trajectories
    def sample_nearby_joint_target(self, q0: np.ndarray) -> np.ndarray:
        for _ in range(50):
            dq = self.env.rng.normal(0.0, np.array([0.25, 0.65, 0.65]))
            q = np.clip(q0 + dq, self.env.joint_min, self.env.joint_max)
            if np.linalg.norm(q - q0) > 0.15:
                return q
        return self.env.compute_random_position()
        
