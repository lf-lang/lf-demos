import numpy as np
from typing import NamedTuple
from ..types.command import Command
import mujoco
from ..mujoco_env.env import MujocoEnv

# structure that contains data related to the input parameters for the PID controller
class Parameters(NamedTuple):
    kp: np.array
    ki: np.array
    kd: np.array
    dt: float
    beta: np.array
    alpha: np.array
    u_min: np.array
    u_max: np.array
    kaw: np.array

# class that defines a PID controller
class PID:
    # init controller without params
    def __init__(self, num_joints: int, env: MujocoEnv):
        self.env = env
        self.buf = mujoco.MjData(self.env.model)
        self.n = num_joints
        self.kp = np.zeros(self.n)
        self.ki = np.zeros(self.n)
        self.kd = np.zeros(self.n)
        self.dt = np.zeros(self.n)
        self.beta = np.zeros(self.n)
        self.alpha = np.zeros(self.n)
        self.prev_i = np.zeros(self.n)
        self.prev_error = np.zeros(self.n)
        self.prev_meas = np.zeros(self.n)
        self.d_filt = np.zeros(self.n)
        self.error = np.zeros(self.n)
        self.prev_u_raw = np.zeros(self.n)
        self.prev_u_sat = np.zeros(self.n)
        self.gear_ratios = np.array([15, 5, 5])
        self.saturator = None
        self.antiwind = None
    
    # set params for the controller, this method is used for ppo interfacing with pid
    def set_params(self, kp: np.array, ki: np.array, kd: np.array, dt: float, beta: np.array, alpha: np.array, u_min: np.array, u_max: np.array, kaw: np.array):
        if kp.size != self.n:
            raise ValueError(f"kp must have size {self.n}, got {kp.size}")
        if ki.size != self.n:
            raise ValueError(f"ki must have size {self.n}, got {ki.size}")
        if kd.size != self.n:
            raise ValueError(f"kd must have size {self.n}, got {kd.size}")
        if beta.size != self.n:
            raise ValueError(f"beta must have size {self.n}, got {beta.size}")
        if alpha.size != self.n:
            raise ValueError(f"alpha must have size {self.n}, got {alpha.size}")
        if u_min.size != self.n:
            raise ValueError(f"u_min must have size {self.n}, got {u_min.size}")
        if u_max.size != self.n:
            raise ValueError(f"u_max must have size {self.n}, got {u_max.size}")
        if kaw.size != self.n:
            raise ValueError(f"kaw must have size {self.n}, got {kaw.size}")
        if dt <= 0.0:
            raise ValueError("dt must be positive")
        if np.any(alpha < 0.0) or np.any(alpha > 1.0):
            raise ValueError("alpha must be in (0, 1)")
        if np.any(u_min > u_max):
            raise ValueError("u_min must be <= u_max elementwise")
        
        self.kp = kp
        self.ki = ki
        self.kd = kd
        self.dt = dt
        self.beta = beta
        self.alpha = alpha
        self.saturator = Saturation(u_min, u_max)
        self.antiwind = AntiWindup(kaw)

    # compute the pid output effort for the controller based on the current state of the joint space and the desired state of the joint space
    def compute_effort(self, enable_pid: bool, enable_grav_comp: bool, enable_inv_dyn: bool, use_weighted_error:bool, q_des: np.array, q_meas: np.array, qd_meas: np.array, qd_des: np.array, qdd_des: np.array) -> np.array:
        if not enable_pid:
            self.prev_error.fill(0.0)
            self.prev_i.fill(0.0)
            self.d_filt.fill(0.0)
            self.prev_meas.fill(0.0)
            return np.zeros(self.n)
        
        # compute error
        self.error = q_des - q_meas
        weighted_error = (self.beta * q_des) - q_meas

        if use_weighted_error:
            P = self.kp * weighted_error
        else:
            P = self.kp * self.error

        # treat d term as damping term; smooths control effort
        d_raw = -qd_meas

        # if use_deriv_on_meas:
        #     d_raw = (self.prev_meas - measurement) / self.dt
        # else:
        #     d_raw = (self.error - self.prev_error) / self.dt

        # construct d term using filtering so that control effort isn't affected heavily by sudden changes
        self.d_filt = self.alpha * self.d_filt + (1 - self.alpha) * d_raw
        D = self.kd * self.d_filt

        # accumulate error history
        I_ = self.prev_i + self.ki * self.error * self.dt

        # these ended up not being used, but can be used to show how adding model-specific control affects the pid controller
        if (enable_grav_comp):
            ff = self.compute_grav_comp_effort(enable_grav_comp, q_meas)
        elif (enable_inv_dyn):
            ff = self.compute_inv_dyn_effort(enable_inv_dyn, q_des, qd_des, qdd_des)
        else:
            ff = np.zeros(self.n)
            
        # proposed tau term with all pid terms + feedforward
        tau = P + I_ + D + ff
        
        # take down tau by the gear ratio of the motors defined in the model
        u_raw = tau / self.gear_ratios
        
        # clip if saturated
        u_sat = self.saturator.clip(u_raw)

        # back calculate the I term if saturated
        tau_sat = u_sat * self.gear_ratios
        I = self.antiwind.anti_windup_backcalc(I_, self.dt, tau_sat, tau)

        self.prev_error = self.error
        self.prev_i = I
        self.prev_meas = q_meas
        self.prev_u_raw = u_raw

        return Command(torques=u_sat.copy())

    # computes the gravity compensation from model - not to be used in ppo training as it leaks the unknown gravity
    def compute_grav_comp_effort(self, enabled: bool, q_meas: np.array):
        if not enabled:
            return np.zeros(self.n)

        buf = self.buf
        buf.qpos[:] = q_meas
        buf.qvel[:] = 0.0
        buf.qacc[:] = 0.0

        mujoco.mj_forward(self.env.model, buf)

        tau_g = buf.qfrc_bias.copy()
        return tau_g
        
    # full inverse dynamics control
    def compute_inv_dyn_effort(self, enabled: bool, q_des: np.array, qd_des: np.array, qdd_des: np.array) -> np.array:
        if not enabled:
            return np.zeros(self.n)
        
        buf = self.buf
        buf.qpos[:] = q_des
        buf.qvel[:] = qd_des
        buf.qacc[:] = qdd_des

        mujoco.mj_inverse(self.env.model, self.buf)
        tau = self.buf.qfrc_inverse.copy()
        return tau

# this class defines saturation handling
class Saturation:
    def __init__(self, u_min: np.array, u_max: np.array):
        self.u_min = np.array(u_min)
        self.u_max = np.array(u_max)

        if self.u_min.shape != self.u_max.shape:
            raise ValueError("u_min and u_max must match shape")

        if np.any(self.u_min > self.u_max):
            raise ValueError("u_min must be <= u_max elementwise")
    
    # hard clamping technique for saturation handling
    def clip(self, u_raw: np.array) -> np.array:
        return np.clip(u_raw, self.u_min, self.u_max)

# this class defines anti-windup handling
class AntiWindup:
    def __init__(self, kaw: np.array):
        self.kaw = kaw

    # caluclate the I term that combats windup based on saturation
    def anti_windup_backcalc(self, i_star: np.array, dt: float, u_sat: np.array, u_raw: np.array) -> np.array:
        return i_star + (self.kaw * (u_sat - u_raw) * dt)
