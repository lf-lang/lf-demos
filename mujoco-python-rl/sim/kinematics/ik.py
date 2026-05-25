import numpy as np
from ..mujoco_env.env import MujocoEnv
import mujoco

# class for computing inverse kinematics and inverse differential kinematics
class InverseKinematics:
    def __init__(self, env: MujocoEnv):
        self.env = env
        self.q = np.zeros(3)
        self.qd = np.zeros(3)
        self.buffer = mujoco.MjData(self.env.model)

    # take desired end-effector position and initial guess for implementing Gauss-Newton method
    def compute_joint_pos(self, desired_pos: np.ndarray, initial_guess: np.ndarray | None = None, step_tol=1e-6, pos_tol=1e-5, ori_tol=1e-6, iterations=300) -> np.ndarray:
        jacp = np.zeros((3, self.env.model.nv))
        jacr = np.zeros((3, self.env.model.nv))
        #desired_orientation = np.array([0, 0, -1])

        # compute hip position to start from
        hip_guess = np.clip(np.atan2(desired_pos[1], desired_pos[0]), self.env.joint_min[0], self.env.joint_max[0])

        # this is used for soft-restarts i.e. if target is following a trajectory
        if initial_guess is not None:
            initial = np.clip(initial_guess.copy(), self.env.joint_min, self.env.joint_max)
        # otherwise use a static guess
        else:
            initial = np.clip(self.q.copy(), self.env.joint_min, self.env.joint_max)
            if np.allclose(initial, 0.0):
                initial[0] = hip_guess
                initial[1] = 1.57
                initial[2] = -1.0

        q_bias = initial.copy()
        q_bias[0] = hip_guess
        k_bias = 0.02

        # Levenberg–Marquardt (LM) formulation
        lam = 1e-1
        lam_up = 5.0
        lam_down = 0.05

        step_size = 0.2
        prev_err = np.inf
        q_next = initial.copy()
        q_prev = initial.copy()
        converged = False
        it = 0
        self.buffer.qvel[:] = 0

        while(True):
            # compute end-effector pose at the current iteration
            self.buffer.qpos[:] = q_prev
            mujoco.mj_forward(self.env.model, self.buffer)
            mujoco.mj_jacSite(self.env.model, self.buffer, jacp, jacr, self.env.ee_site_id)
            curr_pos = self.buffer.site_xpos[self.env.ee_site_id]
            # curr_R = buffer.site_xmat[self.env.ee_site_id].reshape(3,3)

            # z_local = np.array([0, 0, 1])
            # z_world = curr_R @ z_local

            # compute error between pose at current iteration and desired pose
            e_pos = desired_pos - curr_pos
            e = np.linalg.norm(e_pos)
            #e_ori = np.cross(z_world, desired_orientation)

            # convergence check
            if (e < pos_tol):
                converged = True
                break
            if (it >= iterations):
                converged = False
                break

            # Gauss-Newton / LM form
            A_pos = jacp @ jacp.T + (lam**2) * np.eye(3)
            dq_pos = jacp.T @ np.linalg.solve(A_pos, e_pos)

            # attempt to bias the leg towards a desired leg orientation
            dq_bias = k_bias * (q_bias - q_prev) * min(1.0, e / 0.1)
            dq = dq_pos + dq_bias

            # j_pinv = jacp.T @ np.linalg.solve(A_pos, np.eye(3))
            # N = np.eye(self.env.model.nv) - j_pinv @ jacp

            # A_ori = jacr @ jacr.T + (damping**2) * np.eye(3)
            # dq_ori = N @ jacr.T @ np.linalg.solve(A_ori, e_ori)
            
            q_next = np.clip(q_prev + step_size * dq, self.env.joint_min, self.env.joint_max)

            self.buffer.qpos[:] = q_next
            mujoco.mj_forward(self.env.model, self.buffer)
            test_pos = self.buffer.site_xpos[self.env.ee_site_id]
            test_err = np.linalg.norm(desired_pos - test_pos)

            # adjust how fast method converges based on the error
            if (test_err < e):
                lam *= lam_down
            else:
                lam *= lam_up
            lam = float(np.clip(lam, 1e-6, 1e3))

            # check for stagnant error
            if (abs(prev_err - e) < step_tol and e > pos_tol):
                break
            prev_err = e
            q_prev = q_next
            it += 1

        q_final = np.clip(q_prev, self.env.joint_min, self.env.joint_max)
        if (converged):
            self.q = q_final
        return q_final, converged
    
    # use jacobian formulation to compute the desired joint velocities from a desired end-effector velocity
    def compute_joint_vel(self, desired_vel: np.ndarray):
        damping = 1e-1
        alpha = np.zeros(3)
        alpha[:] = 0.9

        jacp = np.zeros((3, self.env.model.nv))
        jacr = np.zeros((3, self.env.model.nv))
        self.buffer.qpos[:] = self.q
        mujoco.mj_forward(self.env.model, self.buffer)
        mujoco.mj_jacSite(self.env.model, self.buffer, jacp, jacr, self.env.ee_site_id)

        A = jacp @ jacp.T + (damping**2) * np.eye(3)

        qd = jacp.T @ np.linalg.solve(A, desired_vel)
        qdd = np.zeros(3)

        return qd, qdd
