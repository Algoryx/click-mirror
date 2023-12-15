#
#   click client in Python
#   Connects REQ socket to tcp://localhost:5555
#   Sends "HandshakeInit" to server, expects "Handshake" back
#

from argparse import ArgumentParser
from time import time
import os
import signal
from pclick import Client
from pclick import MessageFactory
from pclick.Messaging_pb2 import SensorMessageType, ResetMessageType

def parse_args():
    parser = ArgumentParser(description='Demo client connecting to click server')
    parser.add_argument('--host', metavar='<host>', type=str, default="localhost",
                        help=f'server to connect to, default is localhost')
    parser.add_argument('--port', metavar='<port>', type=str, default="5555",
                        help=f'port to connect to, default is 5555')
    parser.add_argument('--addr', metavar='<addr>', type=str, default="",
                        help=f'set addr. Ie ipc:///tmp/click.ipc. host and port will be ignored')
    parser.add_argument('--range', metavar='<range>', type=int, default=0,
                        help=f'How many extra messages to send/recv, default is 0')
    parser.add_argument('--handshake-init', dest='handshake_init', action="store_true", default=True,
                        help=f'Send handshake init. This is the default')
    parser.add_argument('--sensorrequest', dest='sensorrequest', action="store_true",
                        help=f'Send sensor request message')
    parser.add_argument('--controlmessage', metavar='<value>', type=str, default=None,
                        help=f'send controlmessage with control values set to <value>. Overrides --handshake-init. Examples --controlmessage robot1:0,0;panda2:1,1')
    parser.add_argument('--controltype', metavar='<value>', choices=["Angle", "AngleVelocity", "Torque"], default=None,
                        help=f'How to interpret controlmessage values, ie Angle(radians), AngleVelocity(radians/sec), or Torque(Nm). Default is to interpret each value individually')
    parser.add_argument('--errormessage', dest='errormessage', action="store_true",
                        help=f'Send error message')
    parser.add_argument('--resetmessage', dest='resetmessage', action="store_true",
                        help=f'Send reset message')
    parser.add_argument('--end-with-errormessage', dest='end_with_errormessage', action="store_true",
                        help=f'Send an extra error message before quitting')
    return parser.parse_args()


def send_errormessage(client: Client):
    message = MessageFactory.create_errormessage()
    print("Sending errormessage")
    client.send(message)

def handler(signum, frame):
    os._exit(0)

signal.signal(signal.SIGINT, handler)
args = parse_args()
addr = f"tcp://{args.host}:{args.port}"
if args.addr:
    addr = args.addr

#  Socket to talk to server
client = Client()
print(f"Connecting to click server {addr}")
client.connect(addr)

start_time = time()

if args.controlmessage:
    message = MessageFactory.create_controlmessage()
    # Note: Below code uses the protobuf API directly, which does not protect client code from future protocol changes.
    # The C++ Click API has a client layer adding that protection that has not been implemented in python yet.
    robotargs = args.controlmessage.split(";")
    for arg in robotargs:
        robotname, *values = arg.split(":")
        robot = message.objects[robotname]
        arr = list(map(float, values[0].split(",")))
        if args.controltype is None:
            robot.values.extend(arr)
        elif args.controltype.lower() == "angle":
            robot.angles.extend(arr)
        elif args.controltype.lower() == "anglevelocity":
            robot.angleVelocities.extend(arr)
        elif args.controltype.lower() == "torque":
            robot.torques.extend(arr)
    print(f"Sending {str(message)}")
    client.send(message)
elif args.resetmessage:
    message = MessageFactory.create_resetmessage()
    print("Sending resetmessage")
    client.send(message)
elif args.sensorrequest:
    message = MessageFactory.create_sensorrequestmessage()
    print("Sending sensor request message")
    client.send(message)
elif args.errormessage:
    send_errormessage(client)
    client.stop()
    os._exit(0)
else:
    message = MessageFactory.create_handshake_init()
    print("Sending initiate handshake")
    client.send(message)

response = client.recv()
print(f"Received response {response}")

for i in range(0, args.range):
    client.send(message)
    response = client.recv()
    if (response.messageType == ResetMessageType):
        print("Received ResetMessage")

wall_time = time() - start_time
freq = (args.range + 1) / wall_time
print(f"Sent/received {args.range + 1} messages in {wall_time:.2f} seconds, ie {freq:.0f} Hz")

if args.end_with_errormessage:
    send_errormessage(client)
