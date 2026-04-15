# Lingua Franca Drone Demo
Drone is purchased from [Duckiedrone DD24](https://get.duckietown.com/products/autonomous-raspberrypi-quadcopter-duckiedrone-dd24), built using the official [Duckietown DD24 assembly and configuration guide](https://docs.duckietown.com/daffy/opmanual-dd24/intro.html).

## Main Files

- `src/test.lf`: main hardware demo
- `src/DroneBridgeC.lf`: standalone serial RC bridge experiment
- `../lib/ToFBridgeC.lf`: live ToF sensor bridge
- `../lib/avoid_planner_modal.lf`: modal avoidance and landing logic
- `../lib/msp_sender.lf`: MSP RC sender and logger
- `../lib/UserLandCmd.lf`: keyboard-triggered landing command

# Prerequisites

Fly the drone using the instructions from [Duckietown DD24 assembly and configuration guide](https://docs.duckietown.com/daffy/opmanual-dd24/intro.html)

See the repository root [README.md](../README.md) for the shared software setup and Python dependencies.

In addition, this workflow expects:

- A configured drone platform or compatible test setup
- Connected ToF sensors
- Access to the correct serial device, for example `/dev/ttyACM0`

### To Run the Code

```bash
lfc ./drone/src/test.lf
./drone/bin/test
```

### Additional Instructions
### Additional Instructions

- Before running `lfc`, update the hard-coded absolute import paths in `drone/src/test.lf` so they point to the local files in this repository's `lib/` directory.
- For example, change:

```
import PyToF from "/mnt/e/PhD/lf-demos/lf-drone/lib/ToFBridgeC.lf"
```

to:

```
import PyToF from "../../lib/ToFBridgeC.lf"
```

- The full import block in `drone/src/test.lf` should look like this:

```lf
import PyToF from "../../lib/ToFBridgeC.lf"
import AvoidPlanner from "../../lib/avoid_planner_modal.lf"
import MSPSender from "../../lib/msp_sender.lf"
import UserLandCmd from "../../lib/UserLandCmd.lf"
```

- Verify the bus numbers and I2C addresses in `src/test.lf` for the `bottom`, `front`, `right`, `left`, and `top` sensors.
- Verify the `MSPSender(port="/dev/ttyACM0")` setting for your flight controller.
- Run the executable from the repository root because `ToFBridgeC.lf` launches `python3 ./lib/tof_reader.py`.
- Press `l` while the program is running to request landing through `UserLandCmd`.

### Notes

`src/DroneBridgeC.lf` is useful as a smaller serial-control experiment, but the main end-to-end hardware demo in this directory is `src/test.lf`.