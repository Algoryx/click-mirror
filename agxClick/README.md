# AgxClick

AgxClick uses pClick, AGX and agxBrick to a implement a simulation application that implements Click out of the box for a provided Brick model.

- You can use click_application.py (see below) to load any Brick model and it will find the robots and connect their signals to Click. See Brick Model Requirements below for more info.
- You can Inherit AgxClick.ClickApplication and override it's methods to customize your own application.
- You can connect with any Click client to the simulation.

The flow is the same as for Click

1. Client controller connects and sends HandshakeInit
2. Server responds with Handshake
3. Client receives Handshake and validates the setup.
4. Client sends Controls
5. Server responds with Sensors
6. The loop 4-5 is repeated.

## Install

Prerequisites: AGX and agxBrick (You need to install prior to below commands)

Run below command(s) after the usual AGX `setup_env` command:

```bash
# Latest version
pip install agxClick -U --extra-index-url https://click-access:F2q7LauW_d-HJ7bH37sV@git.algoryx.se/api/v4/projects/262/packages/pypi/simple
# Specific version
pip install agxClick==0.1.9 --extra-index-url https://click-access:F2q7LauW_d-HJ7bH37sV@git.algoryx.se/api/v4/projects/262/packages/pypi/simple
```

## Usage Examples

See [examples](examples)

Run below command(s) after the usual AGX `setup_env` command:

```bash
# Start Click application with a Brick model containing Robots
python3 examples/click_application.py --model testdata/ClickScene.yml:ExampleClickScene
# From different terminal, to get handshake
python3 -m pClick.demo.client
# Step simulation 200 steps and get first simulation step SensorMessage
python3 -m pClick.demo.client --controlmessage "robot1:1,1;robot2:1,1" --range 199 --end-with-errormessage
# It is also possible to run the C++ democlient, but the current version does only support 5 joints so there will be an error message
bin/democlient
```

## Brick Model Requirements

The click application currently supports

- Finding any Robot in the Brick scene
- Sending and receiving robot signals over click messages

In addition, if you want to transfer object pose data for non-robots, you can declare robots and extra components in a clickobjects list, see [ClickScene.yml](testdata/ClickScene.yml] for an example.

## Implementation details

ClickApplication has three major parts
