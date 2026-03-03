# MuJoCo-Python-RL

This demo shows LF working with MuJoCo and PyTorch to train actor-critic networks that tune a PID controller for operation in varying gravitational environments.

## Prerequisites

This demo currently assumes that Python >=3.10 and LF >=0.11 already installed in linux environment.

## Installation

This project requires the dependencies found in requirements.txt.

Start by creating a virtual environment in the project directory
```bash
python3 -m venv .venv
source .venv/bin/activate
```

Check if pip is installed
```bash
pip --version
```

If it is not installed
```bash
python -m pip install --upgrade pip
```

Then
```bash
pip install -r requirements.txt
```

This will install the dependencies necessary for running MuJoCo for the demo

## Compile and Run Demo

Now that the proper dependencies have been installed in the environment, start compilation by running

```bash
lfc lf/Main.lf
python3 src-gen/lf/Main/Main.py
```

If running Main.py fails due to an error similar to

```bash
ModuleNotFoundError: No module named 'src'
```

run this command
```bash
export PYTHONPATH=$PYTHONPATH:$(pwd)
```

and run Main.py again.

To visualize the control of the leg in mujoco, that was generated using LF, run

```bash
python3 src/run_sim.py
```