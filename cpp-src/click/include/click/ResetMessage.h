#pragma once

#include <click/DllExport.h>
#include <click/Message.h>

#include <vector>
#include <memory>

namespace click
{
  namespace protobuf
  {
    class ResetMessage;
  }
}

namespace click
{

  class ResetMessageBuilder;

  class CLICK_EXPORT ResetMessage : public Message
  {
  public:
    MessageType messageType() const;
    std::string debugString() const;

    ~ResetMessage();

  private:
    ResetMessage(std::unique_ptr<protobuf::ResetMessage>);
    virtual std::string serializeToBytes() const;

    std::unique_ptr<protobuf::ResetMessage> pm;

    friend class ResetMessageBuilder;
    friend class MessageSerializer;
  };
}
