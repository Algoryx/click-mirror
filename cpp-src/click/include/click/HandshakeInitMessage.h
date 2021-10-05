#pragma once

#include <click/DllExport.h>
#include <click/Message.h>

#include <vector>
#include <memory>

namespace click
{
  namespace protobuf
  {
    class HandshakeInitMessage;
  }
}

namespace click
{

  class HandshakeInitMessageBuilder;

  class HandshakeInitMessage : public Message
  {
  public:
    CLICK_EXPORT MessageType messageType() const;
    CLICK_EXPORT std::string debugString() const;

    CLICK_EXPORT ~HandshakeInitMessage();

  private:
    HandshakeInitMessage(std::unique_ptr<protobuf::HandshakeInitMessage>);
    virtual std::string serializeToBytes() const;

    std::unique_ptr<protobuf::HandshakeInitMessage> pm;

    friend class HandshakeInitMessageBuilder;
    friend class MessageSerializer;
  };
}
