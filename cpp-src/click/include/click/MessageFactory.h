#pragma once
#include "Messaging.pb.h"

namespace algoryx { namespace click { namespace protobuf {

    class MessageFactory {

        public:
            static ControlMessage * create_controlMessage();
            static HandshakeInitMessage * create_handshakeInitMessage();
    };

}}}
