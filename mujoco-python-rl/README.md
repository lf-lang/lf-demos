# MuJoCo-Python-RL

This demo uses LF and MuJoCo to simulate control of a Hexapod leg with 3 Degrees of Freedom in varying gravitational environments. The joints of the leg are controlled with PID controllers that use a learned gain schedule for consistent operation across user-specified gravitational environments.

## Prerequisites

This demo currently assumes that Python >=3.10 and LF >=0.11 are already installed in a Linux environment.

## Installation

First, clone this repository to your local machine.

This project requires the dependencies found in requirements.txt.

Start by creating a virtual environment in the project directory.
```bash
python3 -m venv .venv
source .venv/bin/activate
```

Check if pip is installed.
```bash
pip --version
```

If it is not installed:
```bash
python -m pip install --upgrade pip
```

Then,
```bash
pip install -r requirements.txt
```

This will install the dependencies necessary for running MuJoCo for the demo.

## Compile and Run Demo

Now that the proper dependencies have been installed in the environment, start compilation by running:

```bash
lfc lf/Main.lf
python3 src-gen/lf/Main/Main.py
```

If running Main.py fails due to an error similar to:

```bash
ModuleNotFoundError: No module named 'src'
```

run this command,
```bash
export PYTHONPATH=$PYTHONPATH:$(pwd)
```

and run Main.py again.

To visualize the control of the leg in MuJoCo, which was generated using LF, run:

```bash
python3 src/run_sim.py
```

## Project Background

This project was inspired by The Luminosity Lab's CHARLOTTE (Crater Hydrogen And Regolith Laboratory for Observation of Technical Terrain Environments) hexapod platform that was designed for exploring craters on the moon. Since the project could only be tested on Earth, it was designed to operate under the assumption of Earth's gravitational acceleration. But, what if we were able to create a controller for the legs that allowed it to operate agnostic to the gravitational acceleration it's experiencing?


## Problem Description

Hexapod locomotion (and most walking robots for that matter) assumes a known direction and magnitude of gravity. These assumptions are baked into the modeling dynamics, controller parameters, and the desired gait for the robot. If a robot designed to operate on Earth uses the same configuration on the Moon, its performance will be unstable. Torque will scale incorrectly, contact forces are altered, the walking gait will behave inconsistently, and the controller performance degrades. A learned gain schedule is proposed to address these issues.

The initial goal for this learned gain schedule is that the leg maintains stable tracking and locomotion under any gravitational acceleration. This can be achieved using Adaptive Model-Free Control. This project proposes using Reinforcement Learning (PPO) to learn the adaptive gain schedule that enables the joint-specific PID controllers to perform consistently across a range of gravitational accelerations.

## Plant

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
