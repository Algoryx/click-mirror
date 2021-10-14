#pragma once
#include <click/DllExport.h>
#include "Messaging.pb.h"
#include <memory>

namespace click { namespace protobuf {

    class MessageFactory {

        public:
            CLICK_EXPORT static ControlMessage * create_controlMessage();
            CLICK_EXPORT static HandshakeInitMessage * create_handshakeInitMessage();
            CLICK_EXPORT static SensorRequestMessage createSensorRequestMessage();
    };

}}
