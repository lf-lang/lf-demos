# mujoco

[MuJoCo](https://mujoco.org) (Multi-Joint dynamics with Contact) is a physics-based simulation engine with graphics and animation.

## Prerequisites

MuJoCo depends on [GLFW](https://www.glfw.org), a graphics library that you must install.  On macOS:

```sh
brew install glfw
```

MuJoCo itself seems to be best installed from source.  The following worked for me on macOS:

```sh
git clone git@github.com:google-deepmind/mujoco.git
cd mujoco
mkdir build
cd build
cmake .. -DCMAKE_INSTALL_PREFIX=/usr/local
cmake --build .
sudo cmake --install .
```

The `sudo` on the last line is required to install it in `/usr/local`, which is what I did.

The [mujoco.cmake](src/include/mujoco.cmake) file will need to be changed if you change either of the above install locations.

## Demos

* FIXME