# Lingua Franca Drone Demo
Drone is purchased from [Duckiedrone DD24](https://get.duckietown.com/products/autonomous-raspberrypi-quadcopter-duckiedrone-dd24), built using the official [Duckietown DD24 assembly and configuration guide](https://docs.duckietown.com/daffy/opmanual-dd24/intro.html).

## Prerequisites
Fly the drone using the instructions from [Duckietown DD24 assembly and configuration guide](https://docs.duckietown.com/daffy/opmanual-dd24/intro.html)

## Requirements
Install vl53l1x to read the ToF sensors. 
```
pip install vl53l1x 
```

## Testing
You can test the flight of the drone using 
```
lfc src/DroneBrdigeC.lf
./src-gen/DroneBridgeC/build/DroneBridgeC
```
To test the ToF sensors, you can do it using
```
lfc src/ToFBridgeC.lf
./src-gen/ToFBridgeC/build/ToFBridgeC
```
