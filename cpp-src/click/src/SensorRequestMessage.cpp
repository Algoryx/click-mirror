#include <Messaging.pb.h>
#include <click/SensorRequestMessage.h>

namespace click
{

  SensorRequestMessage::SensorRequestMessage(std::unique_ptr<protobuf::SensorRequestMessage> control_m)
  {
    this->pm = std::move(control_m);
  };

  std::string SensorRequestMessage::debugString() const
  {
    return this->pm->DebugString();
  }

  MessageType SensorRequestMessage::messageType() const
  {
    return static_cast<MessageType>(pm->messagetype());
  }

  std::string SensorRequestMessage::serializeToBytes() const
  {
    return this->pm->SerializeAsString();
  }

  SensorRequestMessage::~SensorRequestMessage() = default;
}
