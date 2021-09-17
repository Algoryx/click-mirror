#pragma once

#include <click/Message.h>
#include <click/DllExport.h>

namespace algoryx { namespace click {
  class MessageSerializer
  {
  public:
    CLICK_EXPORT bool decodeMessage(const std::string& data, Message& message);
    CLICK_EXPORT std::string encodeMessage(const Message& message);
    
  private:
    bool decodeControlMessage(const std::string& data, Message& outMessage);
    bool decodeSensorMessage(const std::string& data, Message& outMessage);
  };
}}
