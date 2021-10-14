#pragma once
#include <click/DllExport.h>
#include "Messaging.pb.h"
#include <memory>

namespace click { namespace protobuf {

    class MessageFactory {

        public:
            CLICK_EXPORT static ControlMessage createControlMessage();
            CLICK_EXPORT static HandshakeInitMessage createHandshakeInitMessage();
            CLICK_EXPORT static SensorRequestMessage createSensorRequestMessage();
    };

}}
