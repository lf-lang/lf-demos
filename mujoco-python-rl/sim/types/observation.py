from typing import NamedTuple
import numpy as np
from typing import Optional

# structure that contains data related to the state of the leg
class Observation(NamedTuple):
    q: np.ndarray
    qd: np.ndarray
    ee_pos: Optional[np.ndarray] = None
    ee_lin_vel: Optional[np.ndarray] = None
    ee_ang_vel: Optional[np.ndarray] = None
    Jp: Optional[np.ndarray] = None
    contacts: Optional[np.ndarray] = None
    masses : Optional[np.ndarray] = None
    inertias: Optional[np.ndarray] = None
    damping: Optional[np.ndarray] = None
    gravity: Optional[np.ndarray] = None
    seed: Optional[int] = None
    noise_std: Optional[float] = None
