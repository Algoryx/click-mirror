import zmq
from pClick.message_proto_helpers import MessageSerializer, Message
from abc import abstractmethod


class SizeCollector:

    def sendSize(self, len: int):
        pass

    def recvSize(self, len: int):
        pass


class SizeCollectorSizes(SizeCollector):

    send_sizes = set()
    recv_sizes = set()

    def sendSize(self, len: int):
        self.send_sizes.add(len)

    @abstractmethod
    def recvSize(self, len: int):
        self.recv_sizes.add(len)


class Server:
    def __init__(self, addr: str, size_collector=SizeCollector()):
        self.context = zmq.Context()
        self.socket = self.context.socket(zmq.REP)
        self.addr = addr
        self.socket.bind(self.addr)
        self.size_collector = size_collector

    def recv(self) -> Message:
        """
        Non-blocking receive. Returns Message or None if no Message is waiting.
        """
        try:
            serialized = self.socket.recv(flags=zmq.NOBLOCK)
            self.size_collector.recvSize(len(serialized))
            return MessageSerializer.from_bytes(serialized)

        except zmq.Again as e:
            return None

    def recv_blocking(self) -> Message:
        """
        Blocking receive
        """
        return MessageSerializer.from_bytes(self.socket.recv())

    def send(self, message: Message):
        """
        Non-blocking send. Throws Exception if buffer is full
        """
        serialized = message.SerializeToString()
        self.size_collector.sendSize(len(serialized))
        return self.socket.send(serialized, flags=zmq.NOBLOCK)

    def stop(self):
        self.socket.close()
        self.context.destroy()
