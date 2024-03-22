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

  class CLICK_EXPORT SensorRequestMessage : public Message
  {
  public:
    MessageType messageType() const;
    std::string debugString() const;

    ~SensorRequestMessage();

  private:
    SensorRequestMessage(std::unique_ptr<protobuf::SensorRequestMessage>);
    virtual std::string serializeToBytes() const;

    std::unique_ptr<protobuf::SensorRequestMessage> pm;

    friend class SensorRequestMessageBuilder;
    friend class MessageSerializer;
  };
}
