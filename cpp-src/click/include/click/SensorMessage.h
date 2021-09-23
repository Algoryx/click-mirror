#pragma once
#include <Messaging.pb.h>
#include <click/DllExport.h>
#include <click/Message.h>

#include <vector>

namespace algoryx { namespace click {
  class SensorMessage : public Message
  {
  public:
    CLICK_EXPORT std::vector<double> angles(const std::string &objectname) const;
    CLICK_EXPORT std::vector<double> angleVelocities(const std::string &objectname) const;
    CLICK_EXPORT std::vector<double> torques(const std::string &objectname) const;
    // TODO: Return std::array<double, 3> typedef:ed?
    CLICK_EXPORT std::vector<double> objectRPY(const std::string &objectname) const;
    CLICK_EXPORT std::vector<double> objectPosition(const std::string &objectname) const;
    CLICK_EXPORT std::vector<double> sensor(const std::string &objectname, const std::string &sensorname, int idx) const;
    CLICK_EXPORT MessageType messageType() const;
    CLICK_EXPORT std::string debugString() const;
    CLICK_EXPORT ~SensorMessage();

  private:
    SensorMessage(std::unique_ptr<protobuf::SensorMessage> sensorMessage);
    virtual std::string serializeToBytes() const;

    std::unique_ptr<protobuf::SensorMessage> sensorMess;

    friend class MessageSerializer;
  };

  std::unique_ptr<SensorMessage> toSensorMessage(std::unique_ptr<Message> message);
}}
