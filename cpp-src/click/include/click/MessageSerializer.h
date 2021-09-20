#pragma once

#include <click/Message.h>
#include <click/DllExport.h>

namespace algoryx { namespace click {
    class MessageSerializer
    {
    public:
      CLICK_EXPORT std::string serializeToString(const Message &message);
    };
}}
