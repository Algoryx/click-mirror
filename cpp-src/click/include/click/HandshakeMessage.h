#pragma once
#include <click/DllExport.h>
#include <click/Message.h>

#include <vector>
#include <string>

namespace click
{
  namespace protobuf
  {
    class HandshakeMessage;
  }
  class HandshakeMessageBuilder;
  class HandshakeMessage : public Message
  {
  public:
    struct SimulationSettings
    {
      double timeStep;
    };

    CLICK_EXPORT std::vector<std::string> objects() const;
    CLICK_EXPORT std::vector<std::string> controlsInOrder(const std::string &objectname) const;
    CLICK_EXPORT std::vector<ValueType> controlTypesInOrder(const std::string &objectname) const;
    CLICK_EXPORT std::vector<std::string> jointSensorsInOrder(const std::string &objectname) const;
    CLICK_EXPORT std::vector<ValueType> jointSensors(const std::string &objectname) const;
    CLICK_EXPORT std::vector<ValueType> sensors(const std::string &objectname, const std::string &sensorname) const;
    CLICK_EXPORT std::vector<ValueType> objectSensors(const std::string &objectname) const;
    CLICK_EXPORT std::vector<std::string> controlEvents(const std::string &objectname) const;
    CLICK_EXPORT ValueType controlEvent(const std::string &objectname, const std::string &eventname) const;
    CLICK_EXPORT SimulationSettings simulationSettings() const;
    CLICK_EXPORT ValueType controlType() const;
    CLICK_EXPORT MessageType messageType() const;
    CLICK_EXPORT std::string debugString() const;
    CLICK_EXPORT ~HandshakeMessage();

  private:
    HandshakeMessage(std::unique_ptr<protobuf::HandshakeMessage> HandshakeMessage);
    virtual std::string serializeToBytes() const;

    std::unique_ptr<protobuf::HandshakeMessage> pm;

    friend class MessageSerializer;
    friend class HandshakeMessageBuilderImpl;
  };

  CLICK_EXPORT std::unique_ptr<HandshakeMessage> toHandshakeMessage(std::unique_ptr<Message> message);
}
