#pragma once
#include <Messaging.pb.h>
#include <click/DllExport.h>
#include <click/Message.h>

#include <vector>
#include <array>

namespace click {

  typedef std::array<double, 3> Vec3;

  // TODO: Perhaps wrap union in separate class, and add valueType to know which is set?  
  union Sensor {
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

    // TODO: Use Vec3 below:
    CLICK_EXPORT std::vector<double> objectRPY(const std::string &objectname) const;
    CLICK_EXPORT std::vector<double> objectPosition(const std::string &objectname) const;

    CLICK_EXPORT std::vector<Sensor> sensor(const std::string &objectname, const std::string &sensorname) const;
    CLICK_EXPORT MessageType messageType() const;
    CLICK_EXPORT std::string debugString() const;
    CLICK_EXPORT ~SensorMessage();

  private:
    SensorMessage(std::unique_ptr<protobuf::SensorMessage> sensorMessage);
    virtual std::string serializeToBytes() const;

    // TODO: AGX dev guidelines says use m_ for privates, think about it.
    std::unique_ptr<protobuf::SensorMessage> sensorMess;

    friend class MessageSerializer;
  };

  std::unique_ptr<SensorMessage> toSensorMessage(std::unique_ptr<Message> message);
}
