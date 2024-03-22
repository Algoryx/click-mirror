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

  class CLICK_EXPORT HandshakeInitMessage : public Message
  {
  public:
    MessageType messageType() const;
    std::string debugString() const;

    ~HandshakeInitMessage();

  private:
    HandshakeInitMessage(std::unique_ptr<protobuf::HandshakeInitMessage>);
    virtual std::string serializeToBytes() const;

    std::unique_ptr<protobuf::HandshakeInitMessage> pm;

    friend class HandshakeInitMessageBuilder;
    friend class MessageSerializer;
  };
}
