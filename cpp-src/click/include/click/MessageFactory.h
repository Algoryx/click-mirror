#pragma once
#include <click/DllExport.h>
#include "Messaging.pb.h"
#include <memory>

namespace click { namespace protobuf {

    class CLICK_EXPORT MessageFactory {

        public:
            static ControlMessage createControlMessage();
            static HandshakeInitMessage createHandshakeInitMessage();
            static SensorRequestMessage createSensorRequestMessage();
    };

}}
