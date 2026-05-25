import mujoco
import numpy as np
from ..types.observation import Observation
from .disturbance import DisturbanceGenerator

# this class defines a MuJoCo environment
class MujocoEnv:
    def __init__(self, model_path: str, dt: float = 0.002):
        # define leg model and timestep
        self.model = mujoco.MjModel.from_xml_path(model_path)
        self.model.opt.timestep = dt
        self.data = mujoco.MjData(self.model)
        self.ee_site_id = mujoco.mj_name2id(self.model, mujoco.mjtObj.mjOBJ_SITE, "ee_site")
        self.jacp = np.zeros((3, self.model.nv))
        self.jacr = np.zeros((3, self.model.nv))
        self.rng = None
        self.joint_min = np.array([-0.78, 0.0, -2.1])
        self.joint_max = np.array([0.78, 3.14, 2.1])
        self.nominal_body_mass = self.model.body_mass.copy()
        self.nominal_body_inertia = self.model.body_inertia.copy()
        self.nominal_dof_damping = self.model.dof_damping.copy()
        self.nominal_gravity = self.model.opt.gravity.copy()
        self.gravity_mag_range = (0.0, 15.0)
        self.tilt_xy_range = (-0.5, 0.5)
        self.model_variation_frac = 0.1
        self.disturbance_count_range = (0, 10)
        self.seed = 0
        self.init_pose = np.zeros(3)
        self.masses = np.zeros(self.model.nbody)
        self.inertias = np.zeros(self.model.nbody * 3)
        self.damping = np.zeros(self.model.nv)
        self.gravity = np.array([0.0, 0.0, -9.81])
        self.dist_gen = None
        self.t = 0
        self.noise_std = 0

    # reset the MuJoCo environment with a random seed and return an observation of the environment
    def reset(self, seed: int, random_gravity: bool, random_tilt: bool, vary_model: bool, episode_dur: int, noise_std: float, gravity_override: np.ndarray | None = None) -> Observation:
        # seed env
        self.seed = seed
        self.rng = np.random.default_rng(seed)
        
        # create disturbance generator
        self.dist_gen = DisturbanceGenerator(self, episode_dur)
        dist_min, dist_max = self.disturbance_count_range
        self.dist_gen.generate_schedule(self.rng.integers(dist_min, dist_max), self.rng.integers(dist_min, dist_max), self.rng.integers(dist_min, dist_max))
        self.t = 0
        self.noise_std = noise_std

        # set model params
        mujoco.mj_resetData(self.model, self.data)
        self.model.body_mass[:] = self.nominal_body_mass
        self.model.body_inertia[:] = self.nominal_body_inertia
        self.model.dof_damping[:] = self.nominal_dof_damping
        self.model.opt.gravity[:] = self.nominal_gravity

        # set random initial position of the leg
        rand_pos = self.compute_random_position()
        self.data.qpos[:] = rand_pos
        self.init_pose = rand_pos

        # this section introduces randomization to model params
        self.masses = self.model.body_mass.copy()
        self.inertias = self.model.body_inertia.copy().ravel()
        self.damping = self.model.dof_damping.copy()
        
        # set desired gravity; used for running the LF version of the environment
        if gravity_override is not None:
            gravity_override = np.asarray(gravity_override, dtype=np.float64)
            if gravity_override.shape != (3,):
                raise ValueError(f"gravity_override must have shape (3,), got {gravity_override.shape}")
            self.model.opt.gravity[:] = gravity_override
        else:
            # otherwise vary gravity magnitude and direction for training if specified
            if random_gravity:
                self.model.opt.gravity[:] = np.array([0.0, 0.0, -self.rng.uniform(*self.gravity_mag_range)])
            if random_tilt:
                self.model.opt.gravity[:] = np.array([
                    self.rng.uniform(*self.tilt_xy_range),
                    self.rng.uniform(*self.tilt_xy_range),
                    -self.rng.uniform(*self.gravity_mag_range),
                ])
        self.gravity = self.model.opt.gravity
        
        # vary other model params
        self.model_variation(vary_model)

        # compute initial state in mujoco
        mujoco.mj_forward(self.model, self.data)
        mujoco.mj_jacSite(self.model, self.data, self.jacp, self.jacr, self.ee_site_id)
        v = self.jacp @ self.data.qvel
        w = self.jacr @ self.data.qvel

        # return initial constructed observation
        return Observation(
            q=self.data.qpos.copy(), 
            qd=self.data.qvel.copy(), 
            ee_pos=self.data.site_xpos[self.ee_site_id].copy(), 
            ee_lin_vel=v.copy(), 
            ee_ang_vel=w.copy(), 
            masses=self.masses, 
            inertias=self.inertias, 
            damping=self.damping, 
            gravity=self.gravity, 
            seed=self.seed,
            noise_std=self.noise_std)
    
    # apply torque to the joints of the leg and step the environment
    def step(self, torques: np.ndarray) -> Observation:
        assert torques.shape == (self.model.nu,)
        self.data.qfrc_applied[:] = 0.0
        self.data.xfrc_applied[:] = 0.0

        tau_dist = np.zeros(self.model.nu)
        foot_force = np.zeros(3)

        # inject disturbance if sim running and step corresponds to disturbance schedule
        if self.dist_gen is not None and self.t < self.dist_gen.dur:
            disturbances = self.dist_gen.schedule[self.t] # disturbances at timestep t

            for d in disturbances:
                if d.dist_type in ("impulse", "bias"): # disturbances that affect joints
                    tau_dist += d.magnitude

                elif d.dist_type == "foot_force": # disturbance that affects end-effector
                    foot_force += d.magnitude

        # apply joint disturbance
        self.data.qfrc_applied[:] += tau_dist

        # apply end-effector disturbance
        if np.any(foot_force != 0.0):
            body_id = self.model.site_bodyid[self.ee_site_id]
            wrench = np.zeros(6)
            wrench[:3] = foot_force
            self.data.xfrc_applied[body_id] += wrench

        self.t += 1
        
        # apply torques and step env
        self.data.ctrl[:] = torques
        mujoco.mj_step(self.model, self.data)
        mujoco.mj_jacSite(self.model, self.data, self.jacp, self.jacr, self.ee_site_id)
        v = self.jacp @ self.data.qvel
        w = self.jacr @ self.data.qvel

        return Observation(
            q=self.data.qpos.copy(), 
            qd=self.data.qvel.copy(), 
            ee_pos=self.data.site_xpos[self.ee_site_id].copy(), 
            ee_lin_vel=v.copy(), 
            ee_ang_vel=w.copy(), 
            masses=self.masses, 
            inertias=self.inertias, 
            damping=self.damping, 
            gravity=self.gravity, 
            seed=self.seed,
            noise_std=self.noise_std)
    
    # compute a random initial position upon reset of the environment
    def compute_random_position(self):
        rand_pos_sample = self.rng.random(3,)
        joint_range = self.joint_max - self.joint_min
        rand_pos = self.joint_min + rand_pos_sample * joint_range
        return rand_pos
    
    # vary model params to improve robustness of PPO policy
    def model_variation(self, enabled: bool):
        if not enabled:
            return
        frac = self.model_variation_frac
        self.masses = self.masses + frac * self.masses * self.rng.uniform(-1.0, 1.0) 
        self.inertias = self.inertias + frac * self.inertias * self.rng.uniform(-1.0, 1.0)
        self.damping = self.damping + frac * self.damping * self.rng.uniform(-1.0, 1.0)

        self.model.body_mass[:] = self.masses
        self.model.body_inertia[:] = self.inertias.reshape(self.model.body_inertia.shape)
        self.model.dof_damping[:] = self.damping
