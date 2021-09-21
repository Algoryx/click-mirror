#pragma once

#include <string>
#include<click/DllExport.h>
// NOTE: Include used to keep types consistent with protobuf only
#include<Messaging.pb.h>

namespace algoryx { namespace click {

  enum MessageType {
    HandshakeInitMessageType = protobuf::HandshakeInitMessageType,
    HandshakeMessageType = protobuf::HandshakeMessageType,
    ControlMessageType = protobuf::ControlMessageType,
    SensorMessageType = protobuf::SensorMessageType,
    ResetMessageType = protobuf::ResetMessageType,
    ErrorMessageType = protobuf::ErrorMessageType
  };

  enum ValueType {
    Angle = protobuf::Angle,                   // radians
    AngleVelocity = protobuf::AngleVelocity,   // radians/sec
    Torque = protobuf::Torque,                 // Nm
    Position = protobuf::Position,             // meters vec3
    RPY = protobuf::RPY,                       // radians
    Activated = protobuf::Activated,           // True/False
    Acceleration = protobuf::Acceleration,     // m/s2   vec3
    Force = protobuf::Force                    // Newton vec3
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
