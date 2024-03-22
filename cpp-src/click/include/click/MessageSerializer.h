#pragma once

#include <click/Message.h>
#include <click/DllExport.h>

namespace click {
    class CLICK_EXPORT MessageSerializer
    {
    public:
      std::string serializeToString(const Message& message);
      std::unique_ptr<Message> fromBytes(const std::string& bytes);

      // Below not intended for external usage, but CLICK_EXPORT needed for tests
      std::unique_ptr<Message> sensorMessageFromIStream(std::istream *input);
      std::unique_ptr<Message> handshakeMessageFromIStream(std::istream *input);
    };
}

