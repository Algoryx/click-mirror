#
#   Click server in Python
#   Binds REP socket to tcp://*:5555
#   Expects Message from client, replies with Message
#

import zmq
from message_proto_helpers import MessageFactory, MessageSerializer
from Messaging_pb2 import ControlMessageType, HandshakeInitMessageType, ValueType


def main():
    context = zmq.Context()
    socket = context.socket(zmq.REP)
    socket.bind("tcp://*:5555")

    while True:
        request = MessageSerializer.from_bytes(socket.recv())
        print(f"Received request: {request}")

        if request.messageType == HandshakeInitMessageType:
            response = handshake_message()
            print(f"Sending handshake: {response}")
            socket.send(response.SerializeToString())
        if request.messageType == ControlMessageType:
            response = sensor_message()
            print(f"Sending sensormessage: {response}")
            socket.send(response.SerializeToString())


def handshake_message():
    handshake = MessageFactory.create_handshake()
    handshake.controlType = ValueType.Force
    object = handshake.objects["robot"]

    object.controlsInOrder.extend(["joint1", "joint2"])
    object.controlEvents["gripper"] = ValueType.Activated
    object.controlSensors.extend([ValueType.Angle, ValueType.AngleVelocity, ValueType.Torque])
    object.objectSensors.append(ValueType.Position)
    return handshake


def sensor_message():
    sensor_m = MessageFactory.create_sensormessage()
    robot = sensor_m.objects["robot1"]

    robot.angleSensors.extend([1.0, 1.1])
    robot.angleVelocitySensors.extend([2.0, 2.1])
    robot.torqueSensors.extend([3.0, 3.1])

    box = sensor_m.objects["box"]
    sensor = box.objectSensors.add()
    sensor.position.X = 1.0
    sensor.position.Y = 2.0
    sensor.position.Z = 3.0
    sensor = box.objectSensors.add()
    sensor.rpy.arr.extend([4.0, 5.0, 6.0])
    return sensor_m


main()
