from typing import NamedTuple

# structure that contains data related to state of the environment
class Diagnostics(NamedTuple):
    error_norm: float
    saturated: bool
    contact_flag: bool
