from typing import NamedTuple
from typing import Optional
import numpy as np

# structure that contains data related to the desired states of the leg
class Target(NamedTuple):
    j_targs: Optional[np.ndarray] = None
    task_pos: Optional[np.ndarray] = None