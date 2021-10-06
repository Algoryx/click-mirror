#pragma once
#include <click/DllExport.h>
#include <click/Message.h>

#include <vector>
#include <array>

namespace click
{
  namespace protobuf
  {
    class SensorMessage;
  }

  typedef std::array<double, 3> Vec3;

  // Sensor field/type is defined in handshake
  // TODO: But we should allow asking which anyway.
  union Sensor
  {
    double angle;
    double angleVelocity;
    double torque;
    Vec3 position;
    Vec3 rpy;
    bool activated;
    Vec3 acceleration;
    Vec3 force;
    Vec3 directionalTorque;
    Vec3 angularAcceleration;
  };

  class SensorMessage : public Message
  {
  public:
    CLICK_EXPORT std::vector<double> angles(const std::string &objectname) const;
    CLICK_EXPORT std::vector<double> angleVelocities(const std::string &objectname) const;
    CLICK_EXPORT std::vector<double> torques(const std::string &objectname) const;

    CLICK_EXPORT Vec3 objectRPY(const std::string &objectname) const;
    CLICK_EXPORT Vec3 objectPosition(const std::string &objectname) const;

    CLICK_EXPORT std::vector<Sensor> sensor(const std::string &objectname, const std::string &sensorname) const;
    CLICK_EXPORT Vec3 sensorVec3(const std::string &objectname, const std::string &sensorname, int idx) const;
    CLICK_EXPORT double sensorDouble(const std::string &objectname, const std::string &sensorname, int idx) const;
    CLICK_EXPORT bool sensorBool(const std::string &objectname, const std::string &sensorname, int idx) const;
    CLICK_EXPORT MessageType messageType() const;
    CLICK_EXPORT std::string debugString() const;
    CLICK_EXPORT ~SensorMessage();

  private:
    CLICK_EXPORT SensorMessage(std::unique_ptr<protobuf::SensorMessage>);
    CLICK_EXPORT virtual std::string serializeToBytes() const;

    // TODO: AGX dev guidelines says use m_ for privates, think about it.
    std::unique_ptr<protobuf::SensorMessage> sensorMess;

    friend class MessageSerializer;
  };

  std::unique_ptr<SensorMessage> toSensorMessage(std::unique_ptr<Message> message);
}
