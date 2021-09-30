#pragma once

#include <click/Message.h>
#include <click/DllExport.h>

namespace click {
    class MessageSerializer
    {
    public:
      CLICK_EXPORT std::string serializeToString(const Message &message);
      CLICK_EXPORT std::unique_ptr<Message> fromBytes(const std::string &bytes);

      // Below not intended for external usage
      std::unique_ptr<Message> sensorMessageFromIStream(std::istream *input);
      std::unique_ptr<Message> handshakeMessageFromIStream(std::istream *input);
    };
}

