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

  class ResetMessage : public Message
  {
  public:
    CLICK_EXPORT MessageType messageType() const;
    CLICK_EXPORT std::string debugString() const;

    CLICK_EXPORT ~ResetMessage();

  private:
    ResetMessage(std::unique_ptr<protobuf::ResetMessage>);
    virtual std::string serializeToBytes() const;

    std::unique_ptr<protobuf::ResetMessage> pm;

    friend class ResetMessageBuilder;
    friend class MessageSerializer;
  };
}
