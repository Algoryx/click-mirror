#pragma once

#include <click/DllExport.h>
#include <click/Message.h>
#include <Messaging.pb.h>

#include <vector>
#include <memory>


namespace click {

  class HandshakeInitMessageBuilder;


  class HandshakeInitMessage : public Message
  {
  public:
    CLICK_EXPORT MessageType messageType() const;
    CLICK_EXPORT std::string debugString() const;

    CLICK_EXPORT ~HandshakeInitMessage();

  private:
    CLICK_EXPORT HandshakeInitMessage(std::unique_ptr<protobuf::HandshakeInitMessage>);
    CLICK_EXPORT virtual std::string serializeToBytes() const;

    std::unique_ptr<protobuf::HandshakeInitMessage> pm;

    friend class HandshakeInitMessageBuilder;
    friend class MessageSerializer;
  };
}
