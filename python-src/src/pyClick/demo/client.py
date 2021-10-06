#
#   click client in Python
#   Connects REQ socket to tcp://localhost:5555
#   Sends "HandshakeInit" to server, expects "Handshake" back
#

import zmq
from pyClick.message_proto_helpers import MessageFactory, MessageSerializer
from argparse import ArgumentParser


def parse_args():
    parser = ArgumentParser(description='Demo client connecting to click server')
    parser.add_argument('--host', metavar='<host>', type=str, default="localhost",
                        help=f'server to connect to, default is localhost')
    parser.add_argument('--port', metavar='<port>', type=str, default="5555",
                        help=f'port to connect to, default is 5555')
    return parser.parse_args()


args = parse_args()
addr = f"tcp://{args.host}:{args.port}"

#  Socket to talk to server
socket = zmq.Context().socket(zmq.REQ)
print(f"Connecting to click server {addr}")
socket.connect(addr)

handshake_init = MessageFactory.create_handshake_init()
socket.send(handshake_init.SerializeToString())
responsebytes = socket.recv()
response = MessageSerializer.from_bytes(responsebytes)
print(f"Received response {response}")