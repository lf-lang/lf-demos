# RL-Based Robotic Leg Controller Prototyping and Simulation using Lingua Franca and MuJoCo

**A virtual-prototyping toolchain for PPO-based adaptive PID gain scheduling on a 3-DOF hexapod leg under varying gravity.**

Simulate, evaluate, and retrain a reinforcement-learning controller that adaptively schedules joint-level PID gains for a hexapod leg operating under arbitrary gravitational conditions — from lunar surface gravity to high-gravity exoplanet benchmarks. The control architecture is built with [Lingua Franca](https://lf-lang.org) for deterministic, time-triggered execution around a [MuJoCo](https://mujoco.org) physical plant.

Currently supported on Linux and Mac OS X.

## Capabilities

- **Gravity sweep** — run the full control stack under any gravity vector; the learned policy generalizes across Moon (1.625 m/s²), Earth (9.81 m/s²), and beyond without retuning.
- **Three motion tasks** — pose holding, sinusoidal joint trajectory tracking, and point-to-point end-effector trajectory tracking.
- **Adaptive PID gain scheduling** — a PPO-trained `ActorCritic` network updates all 12 PID gains (Kp, Ki, Kd, Kaw per joint) at 100 Hz from the observed leg state.
- **Domain randomization** — configurable gravity magnitude/direction, body mass, inertia, joint damping, observation noise, observation delay, and random disturbances stress-test controller robustness.
- **Deterministic replay** — Lingua Franca's reactor model of computation ensures every run is bit-reproducible given the same seed and config.
- **Full logging** — every simulation step writes joint states, targets, scheduled gains, torques, and errors to CSV; a summary plot is generated on shutdown.

## Repository Structure

| Folder | Description |
|--------|-------------|
| [configs/](configs/README.md) | JSON training configuration files for PPO runs |
| [sim/](sim/README.md) | Python simulation library (MuJoCo env, PID, IK, RL wrapper, task generators) |
| [src/](src/README.md) | Lingua Franca source files that wire the closed-loop control system |
| [training/](training/README.md) | PPO training scripts (`ppo_agent.py`, `train_ppo.py`) |
| [training/checkpoints/](training/checkpoints/README.md) | Saved PyTorch model checkpoints |
| [logs/](logs/README.md) | Directory to be populated with the LF + MuJoCo simulation results |
| [results/](results/README.md) | Directory with the graphs of the results |

## Prerequisites

- **Operating System**
  - **Linux** (tested on Ubuntu 22.04 and above)
  - **Mac OS X** (see [Troubleshooting Instructions](#troubleshooting-instructions-for-mac-users) for Mac-specific guidlines)
- **Python ≥ 3.10**
- **Java ≥ 17** (required by the Lingua Franca compiler)
- **Lingua Franca ≥ 0.11** (`lfc` on your PATH)

## Installing Lingua Franca

The Lingua Franca (LF) toolchain provides the `lfc` compiler used to build `src/Main.lf`.
To install the LF command line interface (CLI) tools, refer to the [CLI Tools section](https://www.lf-lang.org/docs/installation/#install-script) of the [LF Installation guide](https://www.lf-lang.org/docs/installation/).

For example, this installs the latest release of LF CLI tools:
```
curl -Ls https://install.lf-lang.org | bash -s cli
```

You may need to install some prerequisites for the LF CLI tools as well, using the following commands.
```
sudo apt update
sudo apt install gh git curl openjdk-17-jdk openjdk-17-jre cmake
```

## Installing Python Dependencies

Clone this repository, then create and activate a virtual environment:

```bash
python3 -m venv .venv
source .venv/bin/activate
pip install --upgrade pip
```

Using the following commands, you can install all necessary dependencies including MuJoCo.

### CUDA (NVIDIA GPU)

```bash
pip install -r requirements.txt
```

### CPU-only (no NVIDIA GPU)

Use the separate CPU requirements file, which omits all CUDA/nvidia packages and pulls the CPU-only PyTorch wheel:

```bash
pip install -r requirements-cpu.txt
```

> **Note:** Training is significantly slower without a GPU, but running the pre-trained policies in the LF simulation is fast on CPU since inference over a small MLP is inexpensive.

## Compile and Run Demo

With the virtual environment active and dependencies installed, compile and run:

```bash
lfc src/Main.lf
bin/Main
```

If the run fails with:

```
ModuleNotFoundError: No module named 'sim'
```

export the project root to `PYTHONPATH` first:

```bash
export PYTHONPATH=$PYTHONPATH:$(pwd)
bin/Main
```

To visualize the control of the leg in MuJoCo, which was generated using LF, run:

```bash
python3 sim/run_sim.py
```
## Checkpoints

The `training/checkpoints/` directory contains pre-trained PPO policy weights. These files are required for the LF simulation — removing the directory will cause a runtime error. See [training/checkpoints/README.md](training/checkpoints/README.md) for a description of each checkpoint.

To retrain a policy from scratch using one of the provided configs:

```bash
python3 training/train_ppo.py --config configs/ppo_gain_schedule.json
```

New checkpoints are saved to the path specified by `checkpoint_path` in the config (defaults to `training/checkpoints/`).

To visualize a completed simulation run in MuJoCo, replay the logged trajectory:

```bash
python3 sim/run_sim.py
```

## Troubleshooting instructions for Mac users

If the requirements-cpu.txt throws an error in Mac, manual installation of the packages maybe needed: 
```
pip install numpy
pip install mujoco
pip install torch
pip install matplotlib
```
During running the program, if an error is thrown that reads:
```
RuntimeError: Cannot create a GUI FigureManager outside the main thread using the MacOS backend. Use a non-interactive backend like 'agg' to make plots on worker threads.
```
Use this command:
```
export MPLBACKEND=Agg
```

To visualize the simulation, you may need to use `mjpython` (special launcher for MacOS) instead of `python3`:
```
mjpython sim/run_sim.py
```

To learn more about the `mjpython` command for MacOS, see [this documentation](https://mujoco.readthedocs.io/en/3.1.2/python.html#passive-viewer).

## Simulation Results

After execution of the simulation, the simulation results will be recorded under the [logs/](logs/README.md) directory. The simulation results include the complete trajectory and physical information for each joint/axis in a `.csv` file and the visualization of the trajectory in a `.png` file, as well as the disturbance schedule in a `.json` file.

## Project Background

This project was inspired by The Luminosity Lab's CHARLOTTE (Crater Hydrogen And Regolith Laboratory for Observation of Technical Terrain Environments) hexapod platform, which was designed for exploring craters on the moon. Since the project could only be tested on Earth, it was designed to operate under the assumption of Earth's gravitational acceleration. But, what if we were able to create a controller for the legs that allowed it to operate agnostic to the gravitational acceleration it's experiencing?


## Problem Description

Hexapod locomotion (and most walking robots for that matter) assumes a known direction and magnitude of gravity. These assumptions are baked into the modeling dynamics, controller parameters, and the desired gait for the robot. If a robot designed to operate on Earth uses the same configuration on the Moon, its performance will be unstable. Torque will scale incorrectly, contact forces are altered, the walking gait will behave inconsistently, and the controller performance degrades. A learned gain schedule is proposed to address these issues.

The initial goal for this learned gain schedule is that the leg maintains stable tracking and locomotion under any gravitational acceleration. This can be achieved using Adaptive Model-Free Control. This project proposes using Reinforcement Learning (PPO) to learn the adaptive gain schedule that enables the joint-specific PID controllers to perform consistently across a range of gravitational accelerations.

## Physical Plant using MuJoCo Environment

<p align="center">
<img width="800" height="700" alt="3-DoF Hexapod Leg in MuJoCo environment" src="https://github.com/user-attachments/assets/97b2714c-cdba-439a-a641-7d01fb8637c8" />
</p>

## Control Stack

<img width="3778" height="666" alt="Screenshot 2026-04-28 222703" src="https://github.com/user-attachments/assets/5ea51f53-0237-46c0-8cf4-740ee84c8a22" />

## Failures under Changing Gravity

The following graphs and videos show the leg executing a sinusoidal trajectory in different gravitational environments with static controller gains.
Note that a video for Kepler-452b will not be included as the visual differences aren't as apparent.

- Moon (1.625 m/s^2)
- Earth (9.81 m/s^2)
- Kepler-452b (12.164 m/s^2)

<p align="center">
<img width="2164" height="813" alt="Screenshot 2026-04-28 235133" src="https://github.com/user-attachments/assets/2b7c8870-4684-4c16-9dc1-00d13ecf3cc9" />
</p>

https://github.com/user-attachments/assets/1c421d02-2603-4c9b-9d5d-a39306e25632

https://github.com/user-attachments/assets/064b2875-1499-40c4-9bc9-52cd6610e44c

## Reinforcement Learning (PPO)

This problem can be modeled as a Partially Observable Markov Decision Process. 

Environment
- MuJoCo steps environment simulation
- Training done in episodes
- Gravity direction and magnitude randomized at beginning of episode
- Model parameters varied at beginning of episode to simulate modeling uncertainty
- Noise injected into joint measurements during episode to simulate imperfect state estimation
- Randomized starting position of leg
- Randomized task given to execute
- Disturbances injected at randomly scheduled times to improve robustness

The gain schedule is learned using Proximal Policy Optimization.

The code for training the policy implementing the adaptive gain schedule is provided, but support for running that code is not provided or tested. The policies are saved in the training/checkpoint directory and can be used in the PolicyGainScheduler reactor to test in the LF environment.

## LF Diagram
<p align="center">
<img width="3009" height="725" alt="LF diagram" src="https://github.com/user-attachments/assets/0b1bdfbc-2e30-4339-a61c-b9a9f4806df9" />
</p>

## Results

The following graphs and videos show the leg executing the same sinusoidal trajectory as earlier, but using the adaptive gain schedule learned by PPO. Notice how the performance looks nearly identical across all environments!

<p align="center">
<img width="2164" height="832" alt="Screenshot 2026-04-29 002651" src="https://github.com/user-attachments/assets/f399fdd1-14e0-44f9-b938-5c154c96af9e" />
</p>

https://github.com/user-attachments/assets/bd9aee72-c117-4c66-b656-008d4ba825b7

https://github.com/user-attachments/assets/bc4cff16-2b23-4ba1-affc-2f45bdf32d8b

## Limitations

- Learned gain schedule generalizes controller performance across environments; doesn't prioritize controller performance in all scenarios
- No contact force training
- Only one leg
- No sim-to-real validation

## Next Steps

- Modify reward structure to balance controller performance with generalization
- Further tuning of environment variables and hyperparameters
- Modify Actor-Critic networks to use LSTM instead of MLP
- Training for contact forces
- Implement on hexapod stack
- Validate on hardware
