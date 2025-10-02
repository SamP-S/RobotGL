# RobotGL - Sam Pearson-Smith
## Summary
Simulate and live link a 6-DOF robot arm. Model using DH-parameters and calculate both forward and inverse kinematics.

## Requirements
Install dependencies:
``` bash
sudo apt install cmake pkg-config gdb   # installs build tools
```

Run compile script:
``` bash
./compile.sh clean
```

## Usage
Run executable:
``` bash
./build/robot
```

## Tools
Install X11 deps and setup for docker:
``` bash
sudo apt update
sudo apt install x11-xserver-utils
```
Build and run docker container:
``` bash
cd ./tools/python
docker build -t robotgl_python .
docker run --rm -it robotgl_python
```

## Documeents:
- Do the [Northwestern Modern Robotics Course](https://modernrobotics.northwestern.edu/nu-gm-book-resource/introduction-autoplay/#department)
- Inverse Kinematics [Clemson material](https://opentextbooks.clemson.edu/wangrobotics/chapter/inverse-kinematics/)
- Look into [CoppeliaSim](https://uark-meeg-intro-robo.readthedocs.io/en/latest/coppelia.html).

## License
This project is licensed under the [Apache-2.0](https://www.apache.org/licenses/LICENSE-2.0).
