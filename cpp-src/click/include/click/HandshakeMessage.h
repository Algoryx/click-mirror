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
  class CLICK_EXPORT HandshakeMessage : public Message
  {
  public:
    struct SimulationSettings
    {
      double timeStep;
    };

    std::vector<std::string> objects() const;
    std::vector<std::string> controlsInOrder(const std::string& objectname) const;
    std::vector<ValueType> controlTypesInOrder(const std::string& objectname) const;
    std::vector<std::string> jointSensorsInOrder(const std::string& objectname) const;
    std::vector<ValueType> jointSensors(const std::string& objectname) const;
    std::vector<ValueType> sensors(const std::string& objectname, const std::string& sensorname) const;
    std::vector<ValueType> objectSensors(const std::string& objectname) const;
    std::vector<std::string> controlEvents(const std::string& objectname) const;
    ValueType controlEvent(const std::string& objectname, const std::string& eventname) const;
    SimulationSettings simulationSettings() const;
    ValueType controlType() const;
    MessageType messageType() const;
    std::string debugString() const;
    ~HandshakeMessage();

  private:
    HandshakeMessage(std::unique_ptr<protobuf::HandshakeMessage> HandshakeMessage);
    virtual std::string serializeToBytes() const;

    std::unique_ptr<protobuf::HandshakeMessage> pm;

    friend class MessageSerializer;
    friend class HandshakeMessageBuilderImpl;
  };

  std::unique_ptr<HandshakeMessage> toHandshakeMessage(std::unique_ptr<Message> message);
}
