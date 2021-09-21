#pragma once

#include <string>
#include<click/DllExport.h>

namespace algoryx { namespace click {

  enum MessageType {
    HandshakeInitMessageType = 0,
    HandshakeMessageType = 1,
    ControlMessageType = 2,
    SensorMessageType = 3,
    ResetMessageType = 4,
    ErrorMessageType = 5
  };

  class MessageSerializer;

  class Message
  {
  public:
    CLICK_EXPORT virtual std::string debugString() const = 0;
    CLICK_EXPORT virtual MessageType messageType() const = 0;
    CLICK_EXPORT virtual ~Message() = 0;

  private:
    virtual std::string serializeToBytes() const = 0;
  
  friend class MessageSerializer;
};
}}
