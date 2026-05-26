# src

Lingua Franca (Python target) source files that wire together the full closed-loop control system. Compiling `Main.lf` with the LF toolchain generates the `src-gen/` build artifacts and produces the executable in `bin/`.

## Directory Structure

```
src/
├── Main.lf                        # Top-level reactor: wires all reactors and drives timers
├── analysis/
│   └── Logger.lf                  # Logs joint states, targets, and gains to CSV and PNG
├── control/
│   ├── PolicyGainScheduler.lf     # Runs the PPO-trained policy to schedule PID gains
│   ├── OpenLoopController.lf      # Open-loop torque replay (alternative to PID)
│   ├── pid/
│   │   └── PID.lf                 # PID joint controller reactor
│   └── kinematics/
│       └── InverseKinematics.lf   # IK reactor: maps Cartesian targets to joint space
├── plant/
│   ├── Plant.lf                   # Drives the MuJoCo simulation forward in time
│   ├── ObsNoise.lf                # Injects Gaussian noise into observations
│   └── ObsDelay.lf                # Introduces a configurable observation delay
├── planning/
│   └── TargetGenerator.lf         # Generates end-effector targets for the active task
└── scheduling/
    ├── ObsLatch.lf                 # Latches the latest observation for downstream reactors
    └── TargetLatch.lf             # Latches the latest target for downstream reactors
```
