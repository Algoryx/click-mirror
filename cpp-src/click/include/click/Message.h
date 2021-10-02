#pragma once

#include <string>
#include<click/DllExport.h>

namespace click {

  enum MessageType {
    HandshakeInitMessageType = 0,
    HandshakeMessageType = 1,
    ControlMessageType = 2,
    SensorMessageType = 3,
    ResetMessageType = 4,
    ErrorMessageType = 5
  };

  enum ValueType {
    Angle = 0,                     // radians
    AngleVelocity = 1,             // radians/sec
    Torque = 2,                    // Nm
    Position = 3,                  // meters vec3
    RPY = 4,                       // radians
    Activated = 5,                 // True/False
    Acceleration = 6,              // m/s2   vec3
    Force = 7,                     // Newton vec3
    DirectionalTorque = 8,         // Nm     vec3
    AngularAcceleration = 9        // rad/s2 vec3
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
}
