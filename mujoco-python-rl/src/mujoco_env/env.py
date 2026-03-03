import mujoco
import numpy as np
from ..types.observation import Observation

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
        
    # reset the MuJoCo environment with a random seed and return an observation of the environment
    def reset(self, seed: int) -> Observation:
        self.rng = np.random.default_rng(seed)
        mujoco.mj_resetData(self.model, self.data)
        self.data.qpos[:] = self.compute_random_position()
        mujoco.mj_forward(self.model, self.data)
        mujoco.mj_jacSite(self.model, self.data, self.jacp, self.jacr, self.ee_site_id)
        v = self.jacp @ self.data.qvel
        w = self.jacr @ self.data.qvel
        return Observation(q=self.data.qpos.copy(), qd=self.data.qvel.copy(), ee_pos=self.data.site_xpos[self.ee_site_id].copy(), ee_lin_vel=v.copy(), ee_ang_vel=w.copy(), Jp=self.jacp.copy())
    
    # apply torque to the joints of the leg and step the environment
    def step(self, torques: np.ndarray) -> Observation:
       assert torques.shape == (self.model.nu,)
       self.data.ctrl[:] = torques
       mujoco.mj_step(self.model, self.data)
       mujoco.mj_jacSite(self.model, self.data, self.jacp, self.jacr, self.ee_site_id)
       v = self.jacp @ self.data.qvel
       w = self.jacr @ self.data.qvel
       return Observation(q=self.data.qpos.copy(), qd=self.data.qvel.copy(), ee_pos=self.data.site_xpos[self.ee_site_id].copy(), ee_lin_vel=v.copy(), ee_ang_vel=w.copy(), Jp=self.jacp.copy())
    
    # compute a random initial position upon reset of the environment
    def compute_random_position(self):
        rand_pos_sample = self.rng.random(3,)
        joint_range = self.joint_max - self.joint_min
        rand_pos = self.joint_min + rand_pos_sample * joint_range
        return rand_pos