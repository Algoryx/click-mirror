import zmq
from pClick.message_proto_helpers import MessageSerializer, Message


class Server:
    def __init__(self, addr: str):
        self.context = zmq.Context()
        self.socket = self.context.socket(zmq.REP)
        self.addr = addr
        self.socket.bind(self.addr)

    def recv(self) -> Message:
        """
        Non-blocking recv. Returns False if no Message is waiting
        """
        try:
            return MessageSerializer.from_bytes(self.socket.recv(flags=zmq.NOBLOCK))
        except zmq.Again as e:
            return None

    def recv_blocking(self) -> Message:
        """
        Blocking recv
        """
        return MessageSerializer.from_bytes(self.socket.recv())

    def send(self, message):
        """
        Non-blocking send. Throws Exception if buffer is full
        """
        return self.socket.send(message.SerializeToString(), flags=zmq.NOBLOCK)

    def stop(self):
        self.context.destroy()
