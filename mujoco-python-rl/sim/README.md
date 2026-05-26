# sim

Python simulation library for the 3-DOF hexapod leg. It provides the MuJoCo physics environment, PID joint controller, inverse kinematics solver, task/target generators, and the RL training environment wrapper. Both the standalone training loop and the Lingua Franca reactors import from this package.

## Directory Structure

```
sim/
├── mujoco_env/          # Core MuJoCo environment
│   ├── 3dof_leg.xml     # MuJoCo XML model for the 3-DOF leg
│   ├── env.py           # MujocoEnv — physics stepping and domain randomization
│   └── disturbance.py   # DisturbanceGenerator — random impulse/bias/foot-force injection
├── controllers/
│   └── pid.py           # PID joint controller with saturation and anti-windup
├── kinematics/
│   └── ik.py            # InverseKinematics — IK and differential IK via Levenberg–Marquardt
├── rl/
│   └── gain_schedule_env.py  # GainScheduleEnv — Gym-style RL environment for PPO
├── tasks/
│   └── target_gen.py    # TargetGenerator — pose, sinusoid, and trajectory task targets
├── types/
│   ├── command.py       # Command — torque command struct
│   ├── observation.py   # Observation — sensor observation struct
│   ├── diagnostics.py   # Diagnostics — additional runtime diagnostic fields
│   └── target.py        # Target — desired end-effector position/velocity struct
└── run_sim.py           # Standalone viewer: replays a logged trajectory CSV in MuJoCo
```
