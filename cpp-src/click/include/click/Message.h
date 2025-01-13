#pragma once

#include <string>
#include<click/DllExport.h>

namespace click {

  enum class MessageType {
    HandshakeInitMessageType = 0,
    HandshakeMessageType = 1,
    SensorRequestMessageType = 6,
    ControlMessageType = 2,
    SensorMessageType = 3,
    ResetMessageType = 4,
    ErrorMessageType = 5
  };

  enum class ValueType {
    Angle = 0,                       // radians
    AngularVelocity1D = 1,           // radians/sec
    Torque1D = 2,                    // Nm
    Position = 3,                    // meters vec3
    RPY = 4,                         // radians
    Activated = 5,                   // True/False
    Acceleration3D = 6,              // m/s2   vec3
    Force1D = 12,                    // Newton
    Force3D = 7,                     // Newton vec3
    Torque3D = 8,                    // Nm     vec3
    AngularAcceleration3D = 9,       // rad/s2 vec3
    Velocity3D = 10,                 // m/s    vec3
    AngularVelocity3D = 11,          // rad/s2 vec3
    Multiple = 15                    // Multiple ValueType:s
  };

  class MessageSerializer;

  class CLICK_EXPORT Message
  {
  public:
    virtual std::string debugString() const = 0;
    /**
     * Get the type of this message
     *
     * \return MessageType
     */
    virtual MessageType messageType() const = 0;
    virtual ~Message();

  private:
    virtual std::string serializeToBytes() const = 0;

  friend class MessageSerializer;
};
}
