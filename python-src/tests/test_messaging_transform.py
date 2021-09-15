from typing import List
from messaging_pb2 import Message, HandshakeMessage, SensorMessage, ResetMessage, HandshakeInitMessage
from messaging_pb2 import ValueType
from message_proto_helpers import MessageFactory, MessageSerializer


class ControllableRobot():
    # Brick input signals
    input_signals = []
    suction_cup_signal = None
    # Brick output signals
    torque_sensors = []
    angle_sensors = []
    velocity_sensors = []
    suction_cup_body = None
    # Number of input signals == number of output signals
    num_motors = 0
    name = None
    # TODO: control should be accessible in same object?
    control = ValueType.AngleVelocity
    # TODO: Where does jointnames and suction cup name come from?
    jointnames = ["joint1", "joint2", "joint3", "joint4", "joint5", "joint6", "joint7"]
    grippername = "gripper"


robots = [ControllableRobot()]
robots[0].name = "robot1"
robots[0].input_signals = [0, 1, 2, 3, 4, 5, 6]
robots[0].torque_sensors = [0, 1, 2, 3, 4, 5, 6]
robots[0].angle_sensors = [0, 1, 2, 3, 4, 5, 6]
robots[0].velocity_sensors = [0, 1, 2, 3, 4, 5, 6]
robots[0].suction_cup_body = "dummy"


def test_create_handshake_from_robot():

    handshake = MessageFactory.create_handshake()
    handshake.controlType = robots[0].control
    for robot in robots:
        object = handshake.objects[robot.name]
        object.controlsInOrder.extend(robot.jointnames)
        jointsensors = list()
        if len(robot.torque_sensors) > 0:
            jointsensors.append(ValueType.Force)
        if len(robot.angle_sensors) > 0:
            jointsensors.append(ValueType.Angle)
        if len(robot.velocity_sensors) > 0:
            jointsensors.append(ValueType.AngleVelocity)
        object.controlEvents[robot.grippername] = ValueType.Activated

        for joint in robot.jointnames:
            object.sensors[joint].types.extend(jointsensors)
        if robot.suction_cup_body is not None:
            object.objectSensors.append(ValueType.Position)
    assert len(handshake.SerializeToString()) == 214
    assert str(handshake) == handshake_facit


def create_control_fake_for(robots: List[ControllableRobot]):
    control_m = MessageFactory.create_controlmessage()
    for robot in robots:
        control = control_m.objects[robot.name]
        if robot.control == ValueType.Angle:
            control.angles.extend([1.0, 2.0, 3.0, 4.0, 5.0, 6.0, 7.0])
        elif robot.control == ValueType.AngleVelocity:
            control.angleVelocities.extend([1.0, 2.0, 3.0, 4.0, 5.0, 6.0, 7.0])
        else:
            control.torques.extend([1.0, 2.0, 3.0, 4.0, 5.0, 6.0, 7.0])
        control.controlEvents[robot.grippername] = True
    return control_m


def set_from(target: List[float], source: List[float]):
    for i in range(len(target)):
        target[i] = source[i]


def test_update_robots_from_controlmessage():
    control_m = create_control_fake_for(robots)
    assert len(control_m.SerializeToString()) == 85
    print(str(control_m))
    assert str(control_m) == control_facit

    for robot in robots:
        control = control_m.objects[robot.name]
        if robot.control == ValueType.Angle:
            set_from(robot.input_signals, control.angles)
        elif robot.control == ValueType.AngleVelocity:
            set_from(robot.input_signals, control.angleVelocities)
        else:
            set_from(robot.input_signals, control.torques)
        robot.suction_cup_signal = control.controlEvents[robot.grippername]
    assert robots[0].input_signals[0] == 1.0


def test_create_SensorMessage_from_robots():
    sensor_m = MessageFactory.create_sensormessage()
    for robot in robots:
        sensor_m.objects[robot.name]
        for i, name in enumerate(robot.jointnames):
            sensors = sensor_m.objects[robot.name].sensors[name]
            sensors.sensor.add().angle = robot.angle_sensors[i]
            sensors.sensor.add().angleVelocity = robot.velocity_sensors[i]
            sensors.sensor.add().torque = robot.torque_sensors[i]

    # TODO: Where get the box?
    box = {
        "name": "box",
        "position": [1, 2, 3],
        "rpy": [1, 2, 3]
    }
    box_m = sensor_m.objects[box['name']]
    sensor = box_m.objectSensors.add()
    pos = box['position']
    sensor.position.X = pos[0]
    sensor.position.X = pos[1]
    sensor.position.X = pos[2]
    rpy = box['rpy']
    sensor.rpy.arr.extend(rpy)

    # TODO: 369 byte per mess is a bit much? 369x2000 = 748k/s
    print(sensor_m.SerializeToString())
    assert len(sensor_m.SerializeToString()) == 370
    assert str(sensor_m) == sensor_facit


handshake_facit = """messageType: HandshakeMessageType
version: "0.1"
controlType: AngleVelocity
objects {
  key: "robot1"
  value {
    controlsInOrder: "joint1"
    controlsInOrder: "joint2"
    controlsInOrder: "joint3"
    controlsInOrder: "joint4"
    controlsInOrder: "joint5"
    controlsInOrder: "joint6"
    controlsInOrder: "joint7"
    controlEvents {
      key: "gripper"
      value: Activated
    }
    sensors {
      key: "joint1"
      value {
        types: Force
        types: Angle
        types: AngleVelocity
      }
    }
    sensors {
      key: "joint2"
      value {
        types: Force
        types: Angle
        types: AngleVelocity
      }
    }
    sensors {
      key: "joint3"
      value {
        types: Force
        types: Angle
        types: AngleVelocity
      }
    }
    sensors {
      key: "joint4"
      value {
        types: Force
        types: Angle
        types: AngleVelocity
      }
    }
    sensors {
      key: "joint5"
      value {
        types: Force
        types: Angle
        types: AngleVelocity
      }
    }
    sensors {
      key: "joint6"
      value {
        types: Force
        types: Angle
        types: AngleVelocity
      }
    }
    sensors {
      key: "joint7"
      value {
        types: Force
        types: Angle
        types: AngleVelocity
      }
    }
    objectSensors: Position
  }
}
"""

control_facit = """messageType: ControlMessageType
objects {
  key: "robot1"
  value {
    angleVelocities: 1.0
    angleVelocities: 2.0
    angleVelocities: 3.0
    angleVelocities: 4.0
    angleVelocities: 5.0
    angleVelocities: 6.0
    angleVelocities: 7.0
    controlEvents {
      key: "gripper"
      value: true
    }
  }
}
"""

sensor_facit = """messageType: SensorMessageType
objects {
  key: "box"
  value {
    objectSensors {
      rpy {
        arr: 1.0
        arr: 2.0
        arr: 3.0
      }
    }
  }
}
objects {
  key: "robot1"
  value {
    sensors {
      key: "joint1"
      value {
        sensor {
          angle: 0.0
        }
        sensor {
          angleVelocity: 0.0
        }
        sensor {
          torque: 0.0
        }
      }
    }
    sensors {
      key: "joint2"
      value {
        sensor {
          angle: 1.0
        }
        sensor {
          angleVelocity: 1.0
        }
        sensor {
          torque: 1.0
        }
      }
    }
    sensors {
      key: "joint3"
      value {
        sensor {
          angle: 2.0
        }
        sensor {
          angleVelocity: 2.0
        }
        sensor {
          torque: 2.0
        }
      }
    }
    sensors {
      key: "joint4"
      value {
        sensor {
          angle: 3.0
        }
        sensor {
          angleVelocity: 3.0
        }
        sensor {
          torque: 3.0
        }
      }
    }
    sensors {
      key: "joint5"
      value {
        sensor {
          angle: 4.0
        }
        sensor {
          angleVelocity: 4.0
        }
        sensor {
          torque: 4.0
        }
      }
    }
    sensors {
      key: "joint6"
      value {
        sensor {
          angle: 5.0
        }
        sensor {
          angleVelocity: 5.0
        }
        sensor {
          torque: 5.0
        }
      }
    }
    sensors {
      key: "joint7"
      value {
        sensor {
          angle: 6.0
        }
        sensor {
          angleVelocity: 6.0
        }
        sensor {
          torque: 6.0
        }
      }
    }
  }
}
"""
