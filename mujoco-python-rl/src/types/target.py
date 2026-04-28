from typing import NamedTuple
from typing import Optional
import numpy as np

# structure that contains data related to the desired states of the leg
class Target(NamedTuple):
    pos_targs: Optional[np.ndarray] = None
    vel_targs: Optional[np.ndarray] = None
    acc_targs: Optional[np.ndarray] = None
    task_pos: Optional[np.ndarray] = None
    task_vel: Optional[np.ndarray] = None
    ik_seed: Optional[np.ndarray] = None
