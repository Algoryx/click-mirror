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

  class Message
  {
  public:
    // TODO: Do I need CLICK_EXPORT here?
    CLICK_EXPORT virtual std::string DebugString() const = 0;
    CLICK_EXPORT virtual MessageType messageType() const = 0;
};
}}
