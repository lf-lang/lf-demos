from typing import NamedTuple
import numpy as np

# structure that contains data related to controlling the joints of the leg
class Command(NamedTuple):
    torques: np.ndarray