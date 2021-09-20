#pragma once

#include <click/Message.h>
#include <click/DllExport.h>

namespace algoryx { namespace click {
    class MessageSerializer
    {
    public:
      CLICK_EXPORT std::string serializeToString(const Message &message);
      CLICK_EXPORT Message * fromBytes(const std::string &bytes);
      CLICK_EXPORT Message * fromIStream(std::istream *input);
    };
}}
