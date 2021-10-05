#include <click/MessageFactory.h>

using namespace click::protobuf;

CLICK_EXPORT ControlMessage * MessageFactory::create_controlMessage() {
    ControlMessage * control_m = new ControlMessage();
    control_m->set_messagetype(ControlMessageType);
    return control_m;
}

CLICK_EXPORT HandshakeInitMessage * MessageFactory::create_handshakeInitMessage() {
    HandshakeInitMessage * message = new HandshakeInitMessage();
    message->set_messagetype(HandshakeInitMessageType);
    message->set_version(CURRENT_VERSION);
    return message;
}

