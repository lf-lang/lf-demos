# training

Python PPO (Proximal Policy Optimization) training pipeline. The policy learns to adaptively schedule PID gains for a 3-DOF leg in MuJoCo, adjusting Kp, Ki, Kd, and anti-windup gains in response to the current tracking error, leg state, and task.

## Files

| File | Description |
|------|-------------|
| `ppo_agent.py` | Core PPO components: `ActorCritic` network, `RolloutBuffer`, and `PPOTrainer`. |
| `train_ppo.py` | Training entry point. Loads a config JSON, creates the environment, runs the PPO loop, and saves a checkpoint. |

**Usage:**
In the parent directory run:
```bash
python training/train_ppo.py --config configs/ppo_gain_schedule.json
```

Trained checkpoints are saved to `training/checkpoints/`. Training logs are written to `training/logs/` (created automatically).
