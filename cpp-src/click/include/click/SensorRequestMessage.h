#pragma once

#include <click/DllExport.h>
#include <click/Message.h>

#include <vector>
#include <memory>

namespace click
{
  namespace protobuf
  {
    class SensorRequestMessage;
  }
}

namespace click
{

  class SensorRequestMessageBuilder;

  class SensorRequestMessage : public Message
  {
  public:
    CLICK_EXPORT MessageType messageType() const;
    CLICK_EXPORT std::string debugString() const;

    CLICK_EXPORT ~SensorRequestMessage();

  private:
    SensorRequestMessage(std::unique_ptr<protobuf::SensorRequestMessage>);
    virtual std::string serializeToBytes() const;

    std::unique_ptr<protobuf::SensorRequestMessage> pm;

    friend class SensorRequestMessageBuilder;
    friend class MessageSerializer;
  };
}
