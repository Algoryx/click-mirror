#
#   Click server in Python
#   Binds REP socket to tcp://*:5555
#   Expects Message from client, replies with Message
#

import zmq
from message_proto_helpers import MessageFactory, MessageSerializer
from Messaging_pb2 import Message

context = zmq.Context()
socket = context.socket(zmq.REP)
socket.bind("tcp://*:5555")

while True:
    request = MessageSerializer.from_bytes(socket.recv())
    print(f"Received request: {request}")

    #  Send reply back to client
    response = MessageFactory.create_handshake()
    print(f"Sending response: {response}")
    socket.send(response.SerializeToString())
