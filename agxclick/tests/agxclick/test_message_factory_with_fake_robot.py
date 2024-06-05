from typing import List
from pclick import MessageFactory, ValueType


class ClickRobot():
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
    control = ValueType.AngularVelocity1D
    jointnames: List[str] = []
    grippername: str = ""


robots = [ClickRobot()]
robots[0].name = "robot1"
robots[0].input_signals = [0, 1, 2, 3, 4, 5, 6]
robots[0].torque_sensors = [0, 1, 2, 3, 4, 5, 6]
robots[0].angle_sensors = [0, 1, 2, 3, 4, 5, 6]
robots[0].velocity_sensors = [0, 1, 2, 3, 4, 5, 6]
robots[0].suction_cup_body = "dummy"
robots[0].jointnames = ["joint1", "joint2", "joint3", "joint4", "joint5", "joint6", "joint7"]
robots[0].grippername = "gripper"


def test_create_handshake_from_robot():

    handshake = MessageFactory.create_handshake()
    handshake.controlType = robots[0].control
    for robot in robots:
        object = handshake.objects[robot.name]
        object.controlsInOrder.extend(robot.jointnames)
        object.jointSensorsInOrder.extend(robot.jointnames)
        jointsensors = list()
        if len(robot.angle_sensors) > 0:
            jointsensors.append(ValueType.Angle)
        if len(robot.velocity_sensors) > 0:
            jointsensors.append(ValueType.AngularVelocity1D)
        if len(robot.torque_sensors) > 0:
            jointsensors.append(ValueType.Force3D)
        object.jointSensors.extend(jointsensors)
        object.controlEvents[robot.grippername] = ValueType.Activated
    assert len(handshake.SerializeToString()) == 150
    assert str(handshake) == handshake_facit


def create_control_fake_for(robots: List[ClickRobot]):
    control_m = MessageFactory.create_controlmessage()
    for robot in robots:
        control = control_m.objects[robot.name]
        if robot.control == ValueType.Angle:
            control.angles.extend([1.0, 2.0, 3.0, 4.0, 5.0, 6.0, 7.0])
        elif robot.control == ValueType.AngularVelocity1D:
            control.angularVelocities.extend([1.0, 2.0, 3.0, 4.0, 5.0, 6.0, 7.0])
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
        elif robot.control == ValueType.AngularVelocity1D:
            set_from(robot.input_signals, control.angularVelocities)
        else:
            set_from(robot.input_signals, control.torques)
        robot.suction_cup_signal = control.controlEvents[robot.grippername]
    assert robots[0].input_signals[0] == 1.0


def test_create_SensorMessage_from_robots():
    sensor_m = MessageFactory.create_sensormessage()
    for robot in robots:
        sensors = sensor_m.objects[robot.name]
        sensors.angleSensors.extend(robot.angle_sensors)
        sensors.angularVelocitySensors.extend(robot.velocity_sensors)
        sensors.torqueSensors.extend(robot.torque_sensors)

    box = {
        "name": "box",
        "position": [1, 2, 3],
        "rpy": [1, 2, 3]
    }
    box_m = sensor_m.objects[box['name']]
    sensor = box_m.objectSensors.add()
    pos = box['position']
    sensor.position.arr.extend(pos)
    rpy = box['rpy']
    sensor.rpy.arr.extend(rpy)

    # NOTE: 229 byte per mess: 229x2000 = 458k/s
    print(sensor_m.SerializeToString())
    assert len(sensor_m.SerializeToString()) == 229
    assert str(sensor_m) == sensor_facit


handshake_facit = """messageType: HandshakeMessageType
version: CURRENT_VERSION
controlType: AngularVelocity1D
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
    jointSensorsInOrder: "joint1"
    jointSensorsInOrder: "joint2"
    jointSensorsInOrder: "joint3"
    jointSensorsInOrder: "joint4"
    jointSensorsInOrder: "joint5"
    jointSensorsInOrder: "joint6"
    jointSensorsInOrder: "joint7"
    jointSensors: Angle
    jointSensors: AngularVelocity1D
    jointSensors: Force3D
    controlEvents {
      key: "gripper"
      value: Activated
    }
  }
}
"""

control_facit = """messageType: ControlMessageType
objects {
  key: "robot1"
  value {
    angularVelocities: 1
    angularVelocities: 2
    angularVelocities: 3
    angularVelocities: 4
    angularVelocities: 5
    angularVelocities: 6
    angularVelocities: 7
    controlEvents {
      key: "gripper"
      value: true
    }
  }
}
"""

sensor_facit = """messageType: SensorMessageType
objects {
  key: "robot1"
  value {
    angleSensors: 0
    angleSensors: 1
    angleSensors: 2
    angleSensors: 3
    angleSensors: 4
    angleSensors: 5
    angleSensors: 6
    angularVelocitySensors: 0
    angularVelocitySensors: 1
    angularVelocitySensors: 2
    angularVelocitySensors: 3
    angularVelocitySensors: 4
    angularVelocitySensors: 5
    angularVelocitySensors: 6
    torqueSensors: 0
    torqueSensors: 1
    torqueSensors: 2
    torqueSensors: 3
    torqueSensors: 4
    torqueSensors: 5
    torqueSensors: 6
  }
}
objects {
  key: "box"
  value {
    objectSensors {
      rpy {
        arr: 1
        arr: 2
        arr: 3
      }
    }
  }
}
"""
