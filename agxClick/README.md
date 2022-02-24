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
pip install agxClick==0.1.25 --extra-index-url https://click-access:F2q7LauW_d-HJ7bH37sV@git.algoryx.se/api/v4/projects/262/packages/pypi/simple
```

## Usage Examples

The [examples directory](examples) contain a simple example application, which can be used to run the example scenes in the [testdata directory](testdata).

It is built on top of agxViewer but should be started as a python application, i.e. by running one of

- `python examples/click_application.py` in a Windows environment
- `python3 examples/click_application.py` in a OSX environment or Windows environment with both Python 2 and 3
- `agxpython examples/click_application.py` in a Linux environment.

It adds extra cmdline options to agxViewer which is explained when running with `--help`

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

### Performance

The `--realTime` flag is used to set whether simulation time should be throttled to wall clock time or not.
Setting it to 0 makes the simulation run as fast as possible.

With click:

- Each simulation step is synced with the controller, unless `--disableClickSync` is used
- Every ControlMessage sent from controller will step the simulation one step
- When using --realTime 1, syncing with wall clock is performed)

If you want to run the simulation at full speed without waiting for control messages, use `--disableClickSync`.
WARNING: This means that the results are non-deterministic - but it is also closer to what will happen in a real setting.

Flag                 | Meaning
---------------------|--------------------------------------------------------------------------------------------------
`--realTime 0`       | Do not sync simulation with *wallclock time* - run as fast as possible
`--realTime 1`       | Synchronize simulation with *wallclock time* - a 10 second simulation will take at least 10 seconds
`--disableClickSync` | Do not sync simulation with *controller* - run as fast as possible. (Without this flag, synchronization will occur)
`--framerate 60`     | Update graphics at a separate frequency than simulation.

`--framerate` is useful when simulation timestep is less than 0.167 (60 Hz). It can increase performance for agxViewer (when running agx on host), but typically not for SimulationView (with `--agxOnly`).

If you want a simulation to run as fast as possible, you should use `--realTime 0 --disableClickSync --framerate 60`.
If you want to run as close to realtime/walltime as possible, you should use e.g. `--realTime 1 --disableClickSync --framerate 60`.

### Profiling

Profiling can be done by providing examples with `--profile --stopAfter <seconds>`, e.g. `--profile --stopAfter 10`
You can also add `--profileFile my_file.prof` which will create a file that can be opened with for example [snakeviz](https://jiffyclub.github.io/snakeviz/)

### Batch simulations

The simulation scene can be restarted from the initial position continously and automatically after a specified amount of time.

Add `--batch <seconds>` on the commandline and the ResetBatchListener will reset the simulation after the specified amout of time. If the Brick model include any `BatchVariables` or any `ParameterSpace`, those will also be updated to their next state (redefining the initial state). See [documentation](https://brick:ien1ieh7Cithoohoh2AhNg0waingaigu@d2epgodm7v8ass.cloudfront.net/) for Brick for more information about these types of models.

## Implementation details

ClickApplication has five major parts

- The ClickApplication class itself, which steps the application and simulation in a loop.
- A ClickFrameListener that is envoked each main loop iteration
  - Receives all Click messages, and replies to all immediately except for ControlMessage which is queued for the ClickEventListener
- A ClickEventListener that is envoked each simulation step
  - Updates Robots from the queued ControlMessage before the simulation step.
  - Sends SensorMessage updated from Robots after the simulation step.
- A KeyboardListener that overrides ExampleApplication keys in order to take control of Simulation flow.
- A ResetBatchListener implementing the [Batch Simulation mode](#Batch-Simulations), used to monitor simulation time and ask ClickApplication to reset at intervals
