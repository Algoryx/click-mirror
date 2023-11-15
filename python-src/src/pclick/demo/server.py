#
#   Click server in Python
#   Binds REP socket to tcp://*:5555
#   Expects Message from client, replies with Message
#

import os
import signal
from argparse import ArgumentParser
from pclick import Server, MessageFactory, ControlMessageType, HandshakeInitMessageType, ValueType
from pclick.server import SizeCollectorChanges


def parse_args():
    parser = ArgumentParser(description='Demo client connecting to click server')
    parser.add_argument('--host', metavar='<host>', type=str, default="*",
                        help=f'server to connect to, default is *')
    parser.add_argument('--port', metavar='<port>', type=str, default="5555",
                        help=f'port to connect to, default is 5555')
    parser.add_argument('--addr', metavar='<addr>', type=str, default="",
                        help=f'set addr. Ie ipc:///tmp/click.ipc. host and port will be ignored')
    parser.add_argument('--trace', action='store_true',
                        help=f'print what is sent/received')
    parser.add_argument('--trace-sizes', action='store_true',
                        help=f'print size of what is sent/received')
    return parser.parse_args()

def handler(signum, frame):
    os._exit(0)

def main():
    signal.signal(signal.SIGINT, handler)
    args = parse_args()
    addr = f"tcp://{args.host}:{args.port}"
    if args.addr:
        addr = args.addr
    server = Server(addr)
    if args.trace_sizes:
        server.size_collector = SizeCollectorChanges()

    # Note: Below code uses the protobuf API directly, we recommend using the higher level agxclick ClickObject and ClickRobot instead
    # if possible - to protect client code from future protocol changes.
    while True:

        request = server.recv()
        if request is None:
            continue
        if args.trace:
            print(f"Received request: {request}")

        if request.messageType == HandshakeInitMessageType:
            response = handshake_message()
            if args.trace:
                print(f"Sending handshake: {response}")
            server.send(response)
        if request.messageType == ControlMessageType:
            response = sensor_message()
            if args.trace:
                print(f"Sending sensormessage: {response}")
            server.send(response)
        if args.trace_sizes and server.size_collector.is_updated:
            print(f"Received {server.size_collector.recv_size} from client, Sent {server.size_collector.send_size} bytes to client")


def handshake_message():
    handshake = MessageFactory.create_handshake()
    handshake.controlType = ValueType.Force
    object = handshake.objects["robot"]

    object.controlsInOrder.extend(["joint1", "joint2"])
    object.controlTypesInOrder.extend([ValueType.Angle, ValueType.Angle])
    object.jointSensorsInOrder.extend(["joint1", "joint2"])
    object.controlEvents["gripper"] = ValueType.Activated
    object.jointSensors.extend([ValueType.Angle, ValueType.AngleVelocity, ValueType.Torque])
    object.objectSensors.append(ValueType.Position)
    object.sensors["external_1"].types.extend([ValueType.Force, ValueType.AngularAcceleration])
    return handshake


def sensor_message():
    sensor_m = MessageFactory.create_sensormessage()
    robot = sensor_m.objects["robot1"]

    size = 2
    robot.angleSensors.extend([1.0] * size)
    robot.angleVelocitySensors.extend([2.0] * size)
    robot.torqueSensors.extend([3.0] * size)

    box = sensor_m.objects["box"]
    sensor = box.objectSensors.add()
    sensor.position.arr.extend([1.0, 2.0, 3.0])
    sensor = box.objectSensors.add()
    sensor.rpy.arr.extend([4.0, 5.0, 6.0])
    val = robot.sensors["external_1"].sensor.add()
    val.force.arr.extend([4.0, 4.1, 4.2])
    val = robot.sensors["external_1"].sensor.add()
    val.angularAcceleration.arr.extend([5.0, 5.1, 5.2])

    return sensor_m


main()
