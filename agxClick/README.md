# AgxClick

The main idea behind click is to enable a non-Brick controller talking to a Brick enabled AGX Simulation in way configurable by Brick.
The name comes from the sound two Bricks makes when connected.

There are three main considerations

1. How the controller can send controls and receive sensor values in a similar fashion regardless of environment, ie real or sim.
2. How Brick adds sensors or topology on top of urdf, and how this is communicated to the controller.
3. How to communicate controls and sensors in an effective way.

The current solution is to introduce a Handshake, which enables the simulation to tell the controller what to expect in terms of how to control and what sensor data is being sent.

A typical flow is

1. Client controller connects and sends HandshakeInit
2. Server responds with Handshake
3. Client receives Handshake and validates the setup.
4. Client sends Controls
5. Server responds with Sensors
6. The loop 4-5 is repeated.

## Install

Prerequisites: agx and agxBrick

```bash
# Latest version
pip install agxClick -U --extra-index-url https://click-access:F2q7LauW_d-HJ7bH37sV@git.algoryx.se/api/v4/projects/262/packages/pypi/simple
# Specific version
pip install agxClick==0.1.10 --extra-index-url https://click-access:F2q7LauW_d-HJ7bH37sV@git.algoryx.se/api/v4/projects/262/packages/pypi/simple
```

## Usage Examples

See [examples](examples)

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

In addition, if you want to transfer object pose data, you can declare a clickobjects list, see [ClickScene.yml](testdata/ClickScene.yml] for an example.
