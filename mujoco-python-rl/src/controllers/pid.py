import numpy as np
from typing import NamedTuple
from ..types.command import Command

# structure that contains data related to the input parameters for the PID controller
class Parameters(NamedTuple):
    kp: np.array
    ki: np.array
    kd: np.array
    kff: np.array
    dt: float
    beta: np.array
    alpha: np.array
    u_min: np.array
    u_max: np.array
    kaw: np.array

# class that defines a PID controller
class PID:
    # init controller without params
    def __init__(self, num_joints: int):
        self.n = num_joints
        self.kp = np.zeros(self.n)
        self.ki = np.zeros(self.n)
        self.kd = np.zeros(self.n)
        self.kff = np.zeros(self.n)
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
        self.saturator = None
        self.antiwind = None
    
    # set params for the controller, this method is used in the RL training loop
    def set_params(self, kp: np.array, ki: np.array, kd: np.array, kff: np.array, dt: float, beta: np.array, alpha: np.array, u_min: np.array, u_max: np.array, kaw: np.array):
        if kp.size != self.n:
            raise ValueError(f"kp must have size {self.n}, got {kp.size}")
        if ki.size != self.n:
            raise ValueError(f"ki must have size {self.n}, got {ki.size}")
        if kd.size != self.n:
            raise ValueError(f"kd must have size {self.n}, got {kd.size}")
        if kff.size != self.n:
            raise ValueError(f"kff must have size {self.n}, got {kff.size}")
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
        self.kff = kff
        self.dt = dt
        self.beta = beta
        self.alpha = alpha
        self.saturator = Saturation(u_min, u_max)
        self.antiwind = AntiWindup(kaw)

    # compute the pid output effort for the controller based on the current state of the joint space and the desired state of the joint space
    def compute_pid_effort(self, enabled: bool, use_weighted_error:bool, use_deriv_on_meas: bool, setpoint: np.array, measurement: np.array) -> np.array:
        if not enabled:
            self.prev_error.fill(0.0)
            self.prev_i.fill(0.0)
            self.d_filt.fill(0.0)
            self.prev_meas.fill(0.0)
            return np.zeros(self.n)
        
        self.error = setpoint - measurement
        weighted_error = (self.beta * setpoint) - measurement

        if use_weighted_error:
            P = self.kp * weighted_error
        else:
            P = self.kp * self.error

        if use_deriv_on_meas:
            d_raw = (self.prev_meas - measurement) / self.dt
        else:
            d_raw = (self.error - self.prev_error) / self.dt

        self.d_filt = self.alpha * self.d_filt + (1 - self.alpha) * d_raw
        D = self.kd * self.d_filt

        I_ = self.prev_i + self.ki * self.error * self.dt

        u_raw = P + I_ + D
        u_sat = self.saturator.clip(u_raw)

        I = self.antiwind.anti_windup_backcalc(I_, self.dt, u_sat, u_raw)

        self.prev_error = self.error
        self.prev_i = I
        self.prev_meas = measurement
        self.prev_u_raw = u_raw
        self.prev_u_sat = u_sat

        return u_sat
        
    # compute the feedforward effort for the controller based on the task and desired state of the joint space
    def compute_ff_effort(self, enabled: bool, setpoint: np.array) -> np.array:
        if not enabled:
            return np.zeros(self.n)
        
        return self.saturator.clip(self.kff * setpoint) # disable for now, feedforward will look different than standard velocity control
        
    # compute the effort for the controller combining pid + ff
    def compute_effort(self, use_pid: bool, use_ff: bool, setpoint: np.array, measurement: np.array, use_weighted_error: bool, use_deriv_on_meas: bool) -> np.array:
        assert self.n == setpoint.size and self.n == measurement.size
        effort =  self.saturator.clip(self.compute_pid_effort(use_pid, use_weighted_error, use_deriv_on_meas, setpoint, measurement) + self.compute_ff_effort(use_ff, setpoint))
        return Command(torques=effort.copy())

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