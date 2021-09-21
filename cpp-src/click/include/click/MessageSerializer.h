#pragma once

#include <click/Message.h>
#include <click/DllExport.h>

namespace algoryx { namespace click {
    class MessageSerializer
    {
    public:
      CLICK_EXPORT std::string serializeToString(const Message &message);
      CLICK_EXPORT std::unique_ptr<Message> fromBytes(const std::string &bytes);
      CLICK_EXPORT std::unique_ptr<Message> sensorMessageFromIStream(std::istream *input);
    };
}}
