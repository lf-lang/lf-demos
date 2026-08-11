import numpy as np
from typing import NamedTuple, Optional, TYPE_CHECKING

if TYPE_CHECKING:
    from .env import MujocoEnv

# small clas that defines a disturbance
class Disturbance(NamedTuple):
    time_index: int
    dist_type: str
    joint_id: Optional[int]
    magnitude: np.ndarray
    duration:int

# class that handles generating and scheduling disturbances to affect the plant
class DisturbanceGenerator:
    def __init__(self, env: "MujocoEnv", episode_duration: int):
        self.env = env
        self.dur = episode_duration
        self.schedule: list[list[Disturbance]] = [[] for _ in range(self.dur)]

    def generate_schedule(self, num_impulses: int, num_biases: int, num_forces: int):
        self.schedule = [[] for _ in range(self.dur)]

        # construct impulse disturbances that affect the joints
        for _ in range(num_impulses):
            t = self.env.rng.integers(0, self.dur)

            joint_id = self.env.rng.integers(0, self.env.model.nu)

            magnitude = self.env.rng.normal(0.0, 1.0, size=(self.env.model.nu,))
            magnitude *= 2.0

            disturbance = Disturbance(
                time_index=t,
                dist_type="impulse",
                joint_id=joint_id,
                magnitude=magnitude,
                duration=1
            )

            self.schedule[t].append(disturbance)
        
        # construct bias disturbances that affect the plant
        for _ in range(num_biases):
            t = self.env.rng.integers(0, self.dur // 2)

            duration = self.env.rng.integers(10, 50)

            magnitude = self.env.rng.normal(0.0, 0.5, size=(self.env.model.nu,))

            disturbance = Disturbance(
                time_index=t,
                dist_type="bias",
                joint_id=None,
                magnitude=magnitude,
                duration=duration
            )

            for k in range(t, min(t + duration, self.dur)):
                self.schedule[k].append(disturbance)

        # construct foot disturbances; analagous to an external force hitting the foot
        for _ in range(num_forces):
            t = self.env.rng.integers(0, self.dur // 2)

            duration = self.env.rng.integers(5, 30)

            force = self.env.rng.normal(0.0, 5.0, size=(3,))

            disturbance = Disturbance(
                time_index=t,
                dist_type="foot_force",
                joint_id=None,
                magnitude=force,
                duration=duration
            )

            for k in range(t, min(t + duration, self.dur)):
                self.schedule[k].append(disturbance)
