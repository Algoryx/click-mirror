#include <Messaging.pb.h>
#include <click/ResetMessage.h>

namespace click
{

  ResetMessage::ResetMessage(std::unique_ptr<protobuf::ResetMessage> control_m)
  {
    this->pm = std::move(control_m);
  };

  std::string ResetMessage::debugString() const
  {
    return this->pm->DebugString();
  }

  MessageType ResetMessage::messageType() const
  {
    return static_cast<MessageType>(pm->messagetype());
  }

  std::string ResetMessage::serializeToBytes() const
  {
    return this->pm->SerializeAsString();
  }

  ResetMessage::~ResetMessage() = default;
}
