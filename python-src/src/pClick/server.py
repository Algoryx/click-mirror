import zmq
from pClick.message_proto_helpers import MessageSerializer


class Server:
    def __init__(self, addr: str):
        self.context = zmq.Context()
        self.socket = self.context.socket(zmq.REP)
        self.addr = addr
        self.socket.bind(self.addr)

    def recv(self):
        return MessageSerializer.from_bytes(self.socket.recv())

    def send(self, message):
        return self.socket.send(message.SerializeToString())

    def stop(self):
        self.context.destroy()
