#include <click/MessageFactory.h>
#include <memory>

using namespace click::protobuf;

ControlMessage MessageFactory::createControlMessage() {
    ControlMessage control_m;
    control_m.set_messagetype(ControlMessageType);
    return control_m;
}

HandshakeInitMessage MessageFactory::createHandshakeInitMessage() {
    HandshakeInitMessage message;
    message.set_messagetype(HandshakeInitMessageType);
    message.set_version(CURRENT_VERSION);
    return message;
}

SensorRequestMessage MessageFactory::createSensorRequestMessage() {
    SensorRequestMessage message;
    message.set_messagetype(SensorRequestMessageType);
    return message;
}

