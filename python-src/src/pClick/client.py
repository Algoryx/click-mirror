import zmq
from pclick.message_proto_helpers import MessageSerializer, Message


class Client:
    def __init__(self):
        self.context = zmq.Context()
        self.socket = self.context.socket(zmq.REQ)

    def connect(self, addr: str):
        """
        Asynchronously connects to an endpoint.
        Happily keep trying to connect until there is something there.
        """
        self.socket.connect(addr)

    def recv(self, block=True) -> Message:
        """ Receive a Message. Returns Message, or None if block parameter is True and no Message is waiting.

        block -- whether to wait for message or return directly (default True)
        """
        if block:
            return MessageSerializer.from_bytes(self.socket.recv())
        try:
            return MessageSerializer.from_bytes(self.socket.recv(flags=zmq.NOBLOCK))
        except zmq.Again as e:
            return None

    def send(self, message: Message):
        """ Send a message """
        return self.socket.send(message.SerializeToString())

    def stop(self):
        self.context.destroy()
