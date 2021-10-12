#
#   click client in Python
#   Connects REQ socket to tcp://localhost:5555
#   Sends "HandshakeInit" to server, expects "Handshake" back
#

from pClick import Client
from pClick import MessageFactory
from argparse import ArgumentParser


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
    parser.add_argument('--handshake-init', dest='send_handshake_init', action="store_true",
                        help=f'Send handshake init. This is the default')
    parser.add_argument('--controlmessage', metavar='<value>', type=str, default=None,
                        help=f'send controlmessage with control values set to <value>. Overrides --handshake-init. Examples --controlmessage robot1:0,0;panda2:1,1')
    return parser.parse_args()


args = parse_args()
addr = f"tcp://{args.host}:{args.port}"
if args.addr:
    addr = args.addr

#  Socket to talk to server
socket = Client()
print(f"Connecting to click server {addr}")
socket.connect(addr)

if not args.controlmessage:
    message = MessageFactory.create_handshake_init()
    print(f"Sending initiate handshake")
    socket.send(message)
    response = socket.recv()
else:
    message = MessageFactory.create_controlmessage()
    robotargs = args.controlmessage.split(";")
    for arg in robotargs:
        robotname, *values = arg.split(":")
        robot = message.objects[robotname]
        arr = list(map(float, values[0].split(",")))
        robot.angles.extend(arr)
    print(f"Sending {str(message)}")
    socket.send(message)
    response = socket.recv()

print(f"Received response {response}")

for i in range(0, args.range):
    socket.send(message)
    response = socket.recv()
print(f"Sent {args.range + 1} messages")
