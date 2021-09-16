#pragma once
#include "Messaging.pb.h"

using namespace algoryx::click;

class MessageFactory {

    public:
        static ControlMessage * create_controlMessage();
        static HandshakeInitMessage * create_handshakeInitMessage();
};

