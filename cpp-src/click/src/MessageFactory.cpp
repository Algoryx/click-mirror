#include <click/MessageFactory.h>
#include <memory>

using namespace click::protobuf;

ControlMessage * MessageFactory::create_controlMessage() {
    ControlMessage * control_m = new ControlMessage();
    control_m->set_messagetype(ControlMessageType);
    return control_m;
}

HandshakeInitMessage * MessageFactory::create_handshakeInitMessage() {
    HandshakeInitMessage * message = new HandshakeInitMessage();
    message->set_messagetype(HandshakeInitMessageType);
    message->set_version(CURRENT_VERSION);
    return message;
}

SensorRequestMessage MessageFactory::createSensorRequestMessage() {
    SensorRequestMessage message;
    message.set_messagetype(SensorRequestMessageType);
    return message;
}

