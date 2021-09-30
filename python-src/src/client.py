#
#   click client in Python
#   Connects REQ socket to tcp://localhost:5555
#   Sends "HandshakeInit" to server, expects "Handshake" back
#

import zmq
from message_proto_helpers import MessageFactory, MessageSerializer

context = zmq.Context()

#  Socket to talk to server
print("Connecting to click server…")
print(f"Current libzmq version is {zmq.zmq_version()}")
print(f"Current  pyzmq version is {zmq.__version__}")
socket = context.socket(zmq.REQ)
socket.connect("tcp://localhost:5555")

handshake_init = MessageFactory.create_handshake_init()
socket.send(handshake_init.SerializeToString())
responsebytes = socket.recv()
response = MessageSerializer.from_bytes(responsebytes)
print(f"Received response {response}")
