# MuJoCo-Python-RL

This demo uses LF and MuJoCo to simulate control of a Hexapod leg with 3 Degrees of Freedom in varying gravitational environments. The joints of the leg are controlled with PID controllers that use a learned gain schedule for consistent operation across user-specified gravitational environments.

## Prerequisites

This demo currently assumes that Python >=3.10 and LF >=0.11 already installed in linux environment.

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

To visualize the control of the leg in mujoco, that was generated using LF, run:

```bash
python3 src/run_sim.py
```

## Project Background

This project was inspired by The Luminosity Lab's CHARLOTTE (Crater Hydrogen And Regolith Labortory for Observation of Technical Terrain Environments) hexapod platform that was designed for exploring craters on the moon. Since the project could only be tested on Earth, it was designed to operate under the assumption of Earth's gravitational acceleration. But, what if we were able to create a controller for the legs that allowed it to operate agnostic to the gravitational acceleration it's experiencing?

## Problem Description

Hexapod locomotion (and most walking robots for that matter) assume known direction and magnitude of gravity. These assumptions are baked into the modeling dynamics, controller parameters, and the desired gait for the robot. If a robot designed to operate on Earth uses the same configuration on the Moon, its performance will be unstable. Torque will scale incorrectly, contact forces are altered, the walking gait will behave inconsistenly, and the controller performance degrades. A learned gain schedule is proposed to address these issues.

The initial goal for this learned gain schedule is that the leg maintains stable tracking and locomotion under any gravitational acceleration. This can be achieved using Adaptive Model-Free Control. This project proposes using Reinforcement Learning (PPO) to learn the adaptive gain schedule that enables the joint-specific PID controllers to perform consistenly across a range of gravitational accelerations.

## Plant

