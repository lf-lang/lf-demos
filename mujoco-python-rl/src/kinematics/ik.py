import numpy as np
from ..mujoco_env.env import MujocoEnv
import mujoco

class InverseKinematics:
    def __init__(self, env: MujocoEnv):
        self.env = env

    def compute_joint_pos(self, desired_pos: np.ndarray, step_tol=1e-6, tol=1e-6, iterations=5000) -> np.ndarray:
        jacp = np.zeros((3, self.env.model.nv))
        jacr = np.zeros((3, self.env.model.nv))
        initial = np.zeros(self.env.model.nv)

        hip_guess = np.clip(np.atan2(desired_pos[1], desired_pos[0]), self.env.joint_min[0], self.env.joint_max[0])

        initial[0] = hip_guess

        Rz = np.array([
            [ np.cos(-hip_guess), -np.sin(-hip_guess), 0],
            [ np.sin(-hip_guess),  np.cos(-hip_guess), 0],
            [ 0, 0, 1]
        ])

        p_local = Rz @ desired_pos
        x = p_local[0]
        z = p_local[2]

        L1 = 0.20
        L2 = 0.465

        r = np.sqrt(x**2 + z**2)

        r = np.clip(r, abs(L1 - L2) + 1e-6, L1 + L2 - 1e-6)

        cos_theta = (r**2 - L1**2 - L2**2) / (2 * L1 * L2)
        cos_theta = np.clip(cos_theta, -1.0, 1.0)

        theta = -np.acos(cos_theta)

        alpha = np.atan2(z, x)
        beta = np.atan2(L2 * np.sin(theta), L1 + L2 * np.cos(theta))

        knee_guess = alpha - beta
        ankle_guess = np.pi - theta

        initial[1] = np.clip(knee_guess, self.env.joint_min[1], self.env.joint_max[1])
        initial[2] = np.clip(ankle_guess, self.env.joint_min[2], self.env.joint_max[2])

        damping = 1e-1
        alpha = 1e-2
        q_next = initial.copy()
        q_prev = initial.copy()
        q_bias = np.array([0 , 2.3, 0])
        converged = False
        it = 0
        buffer = mujoco.MjData(self.env.model)
        buffer.qvel[:] = 0

        while(True):
            buffer.qpos[:] = q_prev
            mujoco.mj_forward(self.env.model, buffer)
            mujoco.mj_jacSite(self.env.model, buffer, jacp, jacr, self.env.ee_site_id)
            curr = buffer.site_xpos[self.env.ee_site_id]
            e = desired_pos - curr
            print(it)

            if (np.linalg.norm(e) < tol):
                converged = True
                break
            if (it >= iterations):
                converged = False
                break

            A = jacp.T @ jacp + (damping**2) * np.eye(self.env.model.nv)
            A[1,1] +=alpha
            b = jacp.T @ e
            b[1] += alpha * (q_bias[1] - q_prev[1])
            delta_q = np.linalg.solve(A, b)
            q_next = q_prev + delta_q
            q_next = np.clip(q_next, self.env.joint_min, self.env.joint_max)
            q_prev = q_next
            it += 1

            if (np.linalg.norm(delta_q) < step_tol):
                converged= True
                break

        return q_next, converged
            