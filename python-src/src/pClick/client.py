import zmq
from pClick.message_proto_helpers import MessageSerializer


class Client:
    def __init__(self):
        self.context = zmq.Context()
        self.socket = self.context.socket(zmq.REQ)

    def connect(self, addr: str):
        self.socket.connect(addr)

    def recv(self):
        return MessageSerializer.from_bytes(self.socket.recv())

    def send(self, message):
        return self.socket.send(message.SerializeToString())

    def stop(self):
        self.context.destroy()
