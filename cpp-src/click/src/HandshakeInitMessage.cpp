#include <Messaging.pb.h>
#include <click/HandshakeInitMessage.h>

namespace click
{

  CLICK_EXPORT HandshakeInitMessage::HandshakeInitMessage(std::unique_ptr<protobuf::HandshakeInitMessage> control_m)
  {
    this->pm = std::move(control_m);
  };

  CLICK_EXPORT std::string HandshakeInitMessage::debugString() const
  {
    return this->pm->DebugString();
  }

  CLICK_EXPORT MessageType HandshakeInitMessage::messageType() const
  {
    return static_cast<MessageType>(pm->messagetype());
  }

  std::string HandshakeInitMessage::serializeToBytes() const
  {
    return this->pm->SerializeAsString();
  }

  CLICK_EXPORT HandshakeInitMessage::~HandshakeInitMessage() = default;
}
