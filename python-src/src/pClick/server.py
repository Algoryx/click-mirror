import zmq
from pclick.message_proto_helpers import MessageSerializer, Message
from abc import abstractmethod


class SizeCollector:
    """
    A no operation SizeCollector.
    """

    def sendSize(self, len: int):
        pass

    def recvSize(self, len: int):
        pass


class SizeCollectorChanges(SizeCollector):
    """
    A SizeCollector that enables retrieving changes in send/receive sizes.
    is_updated() returns True if there is a change.
    """
    send_updated = False
    recv_updated = False
    _send_size = None
    _recv_size = None

    @property
    def is_updated(self):
        """
        Returns True if there is a change in send_size or recv_size.
        Side effect: Resets change, i.e. calling is_updated() twice after change will yield True, then False.
        """
        res = self.recv_updated or self.send_updated
        self.send_updated = self.recv_updated = False
        return res

    @property
    def send_size(self):
        return self._send_size

    @property
    def recv_size(self):
        return self._recv_size

    def sendSize(self, len: int):
        assert len > 0
        self.send_updated = len != self._send_size
        self._send_size = len

    def recvSize(self, len: int):
        assert len > 0
        self.recv_updated = len != self._recv_size
        self._recv_size = len


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
