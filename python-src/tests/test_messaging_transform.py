from typing import List
from Messaging_pb2 import Message, HandshakeMessage, SensorMessage, ResetMessage, HandshakeInitMessage
from Messaging_pb2 import ValueType
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
        if len(robot.angle_sensors) > 0:
            jointsensors.append(ValueType.Angle)
        if len(robot.velocity_sensors) > 0:
            jointsensors.append(ValueType.AngleVelocity)
        if len(robot.torque_sensors) > 0:
            jointsensors.append(ValueType.Force)
        object.controlSensors.extend(jointsensors)
        object.controlEvents[robot.grippername] = ValueType.Activated

        if robot.suction_cup_body is not None:
            object.objectSensors.append(ValueType.Position)
    assert len(handshake.SerializeToString()) == 95
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
        sensors = sensor_m.objects[robot.name]
        sensors.angleSensors.extend(robot.angle_sensors)
        sensors.angleVelocitySensors.extend(robot.velocity_sensors)
        sensors.torqueSensors.extend(robot.torque_sensors)

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

    # NOTE: 229 byte per mess: 229x2000 = 458k/s
    print(sensor_m.SerializeToString())
    assert len(sensor_m.SerializeToString()) == 229
    assert str(sensor_m) == sensor_facit


handshake_facit = """messageType: HandshakeMessageType
version: CURRENT_VERSION
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
    controlSensors: Angle
    controlSensors: AngleVelocity
    controlSensors: Force
    controlEvents {
      key: "gripper"
      value: Activated
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
    angleSensors: 0.0
    angleSensors: 1.0
    angleSensors: 2.0
    angleSensors: 3.0
    angleSensors: 4.0
    angleSensors: 5.0
    angleSensors: 6.0
    angleVelocitySensors: 0.0
    angleVelocitySensors: 1.0
    angleVelocitySensors: 2.0
    angleVelocitySensors: 3.0
    angleVelocitySensors: 4.0
    angleVelocitySensors: 5.0
    angleVelocitySensors: 6.0
    torqueSensors: 0.0
    torqueSensors: 1.0
    torqueSensors: 2.0
    torqueSensors: 3.0
    torqueSensors: 4.0
    torqueSensors: 5.0
    torqueSensors: 6.0
  }
}
"""
