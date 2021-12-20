# AgxClick

AgxClick uses pClick, AGX and agxBrick to a implement a simulation application that implements Click out of the box for a provided Brick model.

- You can use click_application.py (see below) to load any Brick model and it will find the robots and connect their signals to Click. See Brick Model Requirements below for more info.
- You can Inherit AgxClick.ClickApplication and override it's methods to customize your own application.
- You can connect with any Click client to the simulation.

The flow is the same as for Click

1. Client controller connects and sends HandshakeInit
2. Server responds with Handshake
3. Client receives Handshake and validates the setup.
4. Client sends ControlMessage
5. Server steps Simulation, and responds with SensorMessage
6. The loop 4-5 is repeated.

NOTE: The Controller step and the simulation step is in full sync, meaning that the simulation will only progress on ControlMessages.

## Install

Prerequisites: AGX and agxBrick (You need to install agxBrick prior to below commands)

```bash
# Latest version
pip install agxClick -U --extra-index-url https://click-access:F2q7LauW_d-HJ7bH37sV@git.algoryx.se/api/v4/projects/262/packages/pypi/simple
# Specific version
pip install agxClick==0.1.17 --extra-index-url https://click-access:F2q7LauW_d-HJ7bH37sV@git.algoryx.se/api/v4/projects/262/packages/pypi/simple
```

## Usage Examples

The [examples directory](examples) contain a simple example application, which can be used to run the example scenes in the [testdata directory](testdata).

### Brick Model Requirements

The click application currently supports

- Finding any Robot in the Brick scene
- Sending and receiving robot signals over click messages

In addition, if you want to transfer object pose data for non-robots, you can declare robots and extra components in a clickobjects list, see [ClickScene.yml](testdata/ClickScene.yml] for an example.

The requirement put on Robots to accomplish the above is

- that each joint has a unique (per robot) protocolReference
- that each sensor not in a joint has a unique (per robot) protocolReference

### ExampleClickScene - Two Robots with Joint sensor and a Box with pose

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

The handshake will include the box and robot debug output as shown below:

```text
objects {
  key: "box"
  value {
    objectSensors: Position
    objectSensors: RPY
  }
}
objects {
  key: "robot1"
  value {
    controlsInOrder: "robot1_joint0"
    controlsInOrder: "robot1_joint1"
    jointSensors: Angle
    jointSensors: AngleVelocity
    jointSensors: Force
    controlEvents {
      key: "gripper"
      value: Activated
    }
    objectSensors: Position
    objectSensors: RPY
    jointSensorsInOrder: "robot1_joint0"
    jointSensorsInOrder: "robot1_joint1"
  }
}
```

Note that the scene specifies unique names for every joint in the joint protocolReference, see [ClickScene.yml](testdata/ClickScene.yml):

```yaml
  robot1:
    .type: ToolRobot
    localTransform:
      position: Math.Vec3(-0.25, 3.45, 0.55)
      rotation: Math.EulerAngles(0, 0, 2.35)
    arm1:
      joint0:
        protocolReference: robot1_joint0
      joint1:
        protocolReference: robot1_joint1
```

### ExampleSensorClickScene - One Robot with external 3DSensor

This scene shows how to use the 3D Force and Torque Sensors.

```bash
# Start simulation
python3 examples/click_application.py --model testdata/ClickScene.yml:ExampleSensorClickScene
# Get handshake
python3 -m pClick.demo.client
# Get sensor message
python3 -m pClick.demo.client --sensorrequest
# Step and get sensor message
python3 -m pClick.demo.client --controlmessage "robot:1,1"
```

Note that the scene uses MyRobot.yml:RobotWith3DSensor which adds the protocolReference:s force-sensor and torque-sensor which needs to be unique per robot:

```yaml
  force3DSensor:
    .type: Sensor.JointForce3DSensor
    protocolReference: forceTorqueSensor
    joint: arm1.joint0

  torque3DSensor:
    .type: Sensor.JointTorque3DSensor
    protocolReference: forceTorqueSensor
    joint: arm1.joint0
```

The handshake contains sensors which was not present in the previous scene.

```bash
python3 -m pClick.demo.client
--- text removed for brevity ---
objects {
  key: "robot"
  value {
    controlsInOrder: "robot_joint0"
    controlsInOrder: "robot_joint1"
    jointSensors: Angle
    jointSensors: AngleVelocity
    jointSensors: Force
    sensors {
      key: "forceTorqueSensor"
      value {
        types: Force
      }
      value {
        types: DirectionalTorque
      }
    }
    objectSensors: Position
    objectSensors: RPY
    jointSensorsInOrder: "robot_joint0"
    jointSensorsInOrder: "robot_joint1"
  }
}
```

The SensorMessage contains gives us the values, all zeroes since no simulation step has been done yet.

```text
python3 -m pClick.demo.client --sensorrequest
--- text removed for brevity ---
objects {
  key: "robot"
  value {
    angleSensors: -0.0
    angleSensors: -0.0
    angleVelocitySensors: 0.0
    angleVelocitySensors: 0.0
    torqueSensors: 0.0
    torqueSensors: 0.0
    objectSensors {
      position {
        arr: 0.0
        arr: 0.0
        arr: 0.0
      }
    }
    objectSensors {
      rpy {
        arr: 0.0
        arr: 0.0
        arr: 0.0
      }
    }
    sensors {
      key: "forceTorqueSensor"
      value {
        sensor {
          force {
            arr: 0.0
            arr: 0.0
            arr: 0.0
          }
        }
      }
      value {
        sensor {
          directionalTorque {
            arr: 0.0
            arr: 0.0
            arr: 0.0
          }
        }
    }
  }
}
```

When stepping with the controlmessage we get sensor values back:

```bash
python3 -m pClick.demo.client --controlmessage "robot:1,1"
--- text removed for brevity ---
    sensors {
      key: "forceTorqueSensor"
      value {
        sensor {
          force {
            arr: 0.0
            arr: 2.7375450446481165e-20
            arr: 39.22636102083025
          }
        }
      value {
        sensor {
          directionalTorque {
            arr: -3.0112845563683974e-20
            arr: 0.0
            arr: 0.0
          }
        }
      }
    }
```

## Implementation details

ClickApplication has four major parts

- The ClickApplication class itself, which steps the application and simulation in a loop.
- A ClickFrameListener that is envoked each main loop iteration
  - Receives all Click messages, and replies to all immediately except for ControlMessage which is queued for the ClickEventListener
- A ClickEventListener that is envoked each simulation step
  - Updates Robots from the queued ControlMessage before the simulation step.
  - Sends SensorMessage updated from Robots after the simulation step.
- A KeyboardListener that overrides ExampleApplication keys in order to take control of Simulation flow.
