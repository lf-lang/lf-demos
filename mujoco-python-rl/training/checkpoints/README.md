# training/checkpoints

Saved PyTorch model checkpoints for the PPO adaptive PID gain scheduler. Each `.pt` file is a dictionary with two keys:
- **`model_state_dict`** — weights for the `ActorCritic` network.
- **`config`** — the raw JSON training config used to produce the checkpoint (includes hyperparameters, gain ranges, reward weights, and task setup).

The checkpoint is loaded by `PolicyGainScheduler.lf` at runtime and by evaluation scripts.

## Checkpoints

| File | Description |
|------|-------------|
| `ppo_gain_schedule.pt` | Baseline mixed-task policy (trajectory 50 %, pose 25 %, sinusoid 25 %) with random gravity and model variation. |
| `ppo_gain_schedule_long.pt` | Extended training run of the baseline mixed-task policy with more timesteps. |
| `ppo_gain_schedule_mixed.pt` | **Primary deployed policy.** Mixed-task policy selected after comparison against a looping-trajectory retrain (see `configs/official_policy.json`). |
| `ppo_gain_schedule_mixed_looping.pt` | Mixed-task retrain candidate with looping trajectory task; outperformed by `ppo_gain_schedule_mixed.pt` on evaluation. |
| `ppo_gain_schedule_pose.pt` | Policy specialized on the static pose-hold task only. |
| `ppo_gain_schedule_sinusoid.pt` | Policy specialized on sinusoidal joint trajectory tracking only. |
| `ppo_gain_schedule_trajectory.pt` | Policy specialized on end-effector waypoint trajectory tracking only. |
| `ppo_gain_schedule_smooth.pt` | Mixed-task policy trained with gain-smoothing and rate-penalty weights for smoother gain transitions. |
| `ppo_gain_schedule_smooth_gravity15.pt` | Smooth policy trained with gravity magnitude randomized up to 15 m/s² for broader domain coverage. |
| `ppo_gain_schedule_smooth_gravity15_tuned.pt` | Fine-tuned variant of `smooth_gravity15` with adjusted reward weights after evaluation. |
| `ppo_smoke.pt` | Checkpoint from a minimal smoke-test run; not intended for deployment. |

