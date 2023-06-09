from pclick.Messaging_pb2 import CURRENT_VERSION, Message, ErrorMessage, HandshakeMessage, SensorMessage, ControlMessage, ResetMessage, HandshakeInitMessage, SensorRequestMessage
from pclick.Messaging_pb2 import HandshakeInitMessageType, ErrorMessageType, HandshakeMessageType, SensorMessageType, ControlMessageType, ResetMessageType, SensorRequestMessageType


class MessageFactory:
    _mdict = {HandshakeInitMessageType: HandshakeInitMessage,
              HandshakeMessageType: HandshakeMessage,
              ControlMessageType: ControlMessage,
              SensorMessageType: SensorMessage,
              ResetMessageType: ResetMessage,
              ErrorMessageType: ErrorMessage,
              SensorRequestMessageType: SensorRequestMessage}

    @classmethod
    def _create_message(cls, mtype):
        message = cls._mdict[mtype]()
        if hasattr(message, "version"):
            message.version = CURRENT_VERSION
        message.messageType = mtype
        return message

    @classmethod
    def create_handshake_init(cls):
        return cls._create_message(HandshakeInitMessageType)

    @classmethod
    def create_handshake(cls):
        return cls._create_message(HandshakeMessageType)

    @classmethod
    def create_controlmessage(cls):
        return cls._create_message(ControlMessageType)

    @classmethod
    def create_sensormessage(cls):
        return cls._create_message(SensorMessageType)

    @classmethod
    def create_resetmessage(cls):
        return cls._create_message(ResetMessageType)

    @classmethod
    def create_errormessage(cls):
        return cls._create_message(ErrorMessageType)

    @classmethod
    def create_sensorrequestmessage(cls):
        return cls._create_message(SensorRequestMessageType)


class MessageSerializer:
    @classmethod
    def from_bytes(cls, bytes: str):
        message = Message()
        message.ParseFromString(bytes)

        m = MessageFactory._mdict[message.messageType]()
        m.ParseFromString(bytes)
        return m
