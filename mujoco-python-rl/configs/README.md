# configs

Training configuration files for the PPO gain scheduling policy. Each file is a JSON object passed to `train_ppo.py` via `--config` and controls both the environment and the PPO hyperparameters for a single training run.

## Files

| File | Description |
|------|-------------|
| `ppo_gain_schedule.json` | Baseline mixed-task run (trajectory 50 %, pose 25 %, sinusoid 25 %) with random gravity/tilt and model variation. |
| `ppo_gain_schedule_long.json` | Same as baseline but with a larger `total_timesteps` budget for extended training. |
| `ppo_gain_schedule_pose.json` | Single-task run focused exclusively on the static pose-hold task. |
| `ppo_gain_schedule_sinusoid.json` | Single-task run focused exclusively on sinusoidal joint trajectory tracking. |
| `ppo_gain_schedule_trajectory.json` | Single-task run focused exclusively on end-effector waypoint trajectory tracking. |
| `ppo_gain_schedule_smooth.json` | Mixed-task run with additional gain-smoothing and rate-penalty weights to produce smoother PID gain transitions. |
| `ppo_gain_schedule_smooth_gravity15.json` | Smooth variant with gravity magnitude range extended to 15 m/s² for more aggressive domain randomization. |
| `ppo_gain_schedule_smooth_gravity15_tuned.json` | Fine-tuned version of the gravity-15 smooth config with reward weights adjusted after evaluation. |
| `ppo_smoke.json` | Minimal smoke-test config with very few timesteps; used to verify training infrastructure without waiting for a full run. |
| `official_policy.json` | Metadata record for the primary deployed checkpoint (`ppo_gain_schedule_mixed.pt`), including a description, selection rationale, and paths to evaluation logs. |

