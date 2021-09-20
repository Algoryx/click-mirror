#pragma once

#include <string>

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
    // TODO: Do I need CLICK_EXPORT here?
    virtual std::string DebugString() const = 0;
    virtual MessageType messageType() const = 0;

  private:
    virtual std::string serializeToBytes() const = 0;
  
  friend class MessageSerializer;
};
}}
