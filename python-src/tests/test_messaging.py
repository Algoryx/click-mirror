from Messaging_pb2 import CURRENT_VERSION, Message, HandshakeMessage, SensorMessage, ResetMessage, HandshakeInitMessage
from Messaging_pb2 import ValueType
from message_proto_helpers import MessageFactory, MessageSerializer
from pathlib import Path
import pytest

# See [Python Generated Code](https://developers.google.com/protocol-buffers/docs/reference/python-generated) 
# for details on how the generated protobuf code is used


def is_valid_message(message: Message):
    return type(message) == MessageFactory._mdict[message.messageType] and \
        message.version == CURRENT_VERSION


def test_understanding_of_protoc_classes():
    # See [Python Generated Code](https://developers.google.com/protocol-buffers/docs/reference/python-generated) for more
    sensormess = MessageFactory.create_sensormessage()
    sobject = sensormess.objects['first']
    assert type(sobject) == SensorMessage.Object

    hobject = HandshakeMessage.Object()
    assert hobject.DESCRIPTOR.full_name == "algoryx.click.protobuf.HandshakeMessage.Object"

    # Misleading that type of two different classes has same name, but that's how this implementation is done.
    assert str(type(hobject)) == str(type(sobject))
    assert str(type(hobject)) == "<class 'Messaging_pb2.Object'>"

    # Enum values are int, not the enum
    assert type(ValueType.Torque) == int


def test_that_Handshake_init_serializes():
    handshake_init = MessageFactory.create_handshake_init()
    assert type(handshake_init) == HandshakeInitMessage

    message = MessageSerializer.from_bytes(handshake_init.SerializeToString())

    assert type(message) == HandshakeInitMessage and is_valid_message(message)


def test_that_Handshake_serializes():
    handshake = MessageFactory.create_handshake()

    message = MessageSerializer.from_bytes(handshake.SerializeToString())

    assert type(message) == HandshakeMessage and is_valid_message(message)


def test_that_Resetmessage_serializes():
    reset = MessageFactory.create_resetmessage()

    message = MessageSerializer.from_bytes(reset.SerializeToString())

    assert type(message) == ResetMessage


def test_that_Handshake_props_are_set():
    handshake = handshake_message()

    message = MessageSerializer.from_bytes(handshake.SerializeToString())
    assert len(handshake.SerializeToString()) ==54

    assert message.controlType == ValueType.Force
    assert message.objects["robot"].controlsInOrder[1] == "joint2"
    assert message.objects["robot"].controlSensors[0] is ValueType.Angle
    assert message.objects["robot"].controlSensors[2] is ValueType.Torque
    assert str(message) == """messageType: HandshakeMessageType
version: CURRENT_VERSION
controlType: Force
objects {
  key: "robot"
  value {
    controlsInOrder: "joint1"
    controlsInOrder: "joint2"
    controlSensors: Angle
    controlSensors: AngleVelocity
    controlSensors: Torque
    controlEvents {
      key: "gripper"
      value: Activated
    }
    objectSensors: Position
  }
}
"""


def handshake_message():
    handshake = MessageFactory.create_handshake()
    handshake.controlType = ValueType.Force
    object = handshake.objects["robot"]

    object.controlsInOrder.extend(["joint1", "joint2"])
    object.controlEvents["gripper"] = ValueType.Activated
    object.controlSensors.extend([ValueType.Angle, ValueType.AngleVelocity, ValueType.Torque])
    object.objectSensors.append(ValueType.Position)
    return handshake


def test_that_ControlMessage_serializes():
    control_m = MessageFactory.create_controlmessage()
    robot1 = control_m.objects["robot1"]

    robot1.angles.extend([1.0, 2.0])
    robot1.controlEvents["gripper"] = True

    message = MessageSerializer.from_bytes(control_m.SerializeToString())
    assert len(control_m.SerializeToString()) == 45

    assert message.objects['robot1'].angles[0] == 1.0
    assert message.objects['robot1'].angles[1] == 2.0
    assert message.objects['robot1'].controlEvents['gripper']
    print(str(message))
    assert str(message) == """messageType: ControlMessageType
objects {
  key: "robot1"
  value {
    angles: 1.0
    angles: 2.0
    controlEvents {
      key: "gripper"
      value: true
    }
  }
}
"""


@pytest.mark.skip
def test_write_sensor_message_to_file(tmp_path: Path):
    filename = tmp_path / "sensormessage.bin"
    filename.write_bytes(sensor_message().SerializeToString())
    print(filename)
    assert False


@pytest.mark.skip
def test_write_handshake_message_to_file(tmp_path: Path):
    filename = tmp_path / "handshakemessage.bin"
    filename.write_bytes(handshake_message().SerializeToString())
    print(filename)
    assert False


def sensor_message():
    sensor_m = MessageFactory.create_sensormessage()
    robot = sensor_m.objects["robot1"]

    robot.angleSensors.extend([1.0, 1.1])
    robot.angleVelocitySensors.extend([2.0, 2.1])
    robot.torqueSensors.extend([3.0, 3.1])

    # sensors = robot.sensors['joint1']
    # sensors.sensor.add().angle = 1.0
    # sensors.sensor.add().angleVelocity = 2.0
    # sensors.sensor.add().torque = 3.0

    # sensors = robot.sensors['joint2']
    # sensors.sensor.add().angle = 1.1
    # sensors.sensor.add().angleVelocity = 2.1
    # sensors.sensor.add().torque = 3.1

    box = sensor_m.objects["box"]
    sensor = box.objectSensors.add()
    sensor.position.X = 1.0
    sensor.position.Y = 2.0
    sensor.position.Z = 3.0
    sensor = box.objectSensors.add()
    sensor.rpy.arr.extend([4.0, 5.0, 6.0])
    return sensor_m


def test_that_SensorMessage_serializes():
    sensor_m = sensor_message()

    message = MessageSerializer.from_bytes(sensor_m.SerializeToString())
    assert len(sensor_m.SerializeToString()) == 138

    assert message.objects['robot1'].angleSensors[0] == 1.0
    assert message.objects['robot1'].torqueSensors[1] == 3.1
    print(str(message))
    assert str(message) == """messageType: SensorMessageType
objects {
  key: "box"
  value {
    objectSensors {
      position {
        X: 1.0
        Y: 2.0
        Z: 3.0
      }
    }
    objectSensors {
      rpy {
        arr: 4.0
        arr: 5.0
        arr: 6.0
      }
    }
  }
}
objects {
  key: "robot1"
  value {
    angleSensors: 1.0
    angleSensors: 1.1
    angleVelocitySensors: 2.0
    angleVelocitySensors: 2.1
    torqueSensors: 3.0
    torqueSensors: 3.1
  }
}
"""
