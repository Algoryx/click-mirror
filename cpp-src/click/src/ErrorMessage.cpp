#include <Messaging.pb.h>
#include <click/ErrorMessage.h>

namespace click
{

  ErrorMessage::ErrorMessage(std::unique_ptr<protobuf::ErrorMessage> control_m)
  {
    this->pm = std::move(control_m);
  };

  std::string ErrorMessage::debugString() const
  {
    return this->pm->DebugString();
  }

  std::string ErrorMessage::message() const
  {
    return this->pm->errormessage();
  }

  MessageType ErrorMessage::messageType() const
  {
    return static_cast<MessageType>(pm->messagetype());
  }

  std::string ErrorMessage::serializeToBytes() const
  {
    return this->pm->SerializeAsString();
  }

  ErrorMessage::~ErrorMessage() = default;
}
