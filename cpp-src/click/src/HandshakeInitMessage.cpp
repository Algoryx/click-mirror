#include <Messaging.pb.h>
#include <click/HandshakeInitMessage.h>

namespace click {

HandshakeInitMessage::HandshakeInitMessage(std::unique_ptr<protobuf::HandshakeInitMessage> control_m)
{
  this->pm = std::move(control_m);
};

std::string HandshakeInitMessage::debugString() const
{
  return this->pm->DebugString();
}

MessageType HandshakeInitMessage::messageType() const {
  return static_cast<MessageType>(pm->messagetype());
}

std::string HandshakeInitMessage::serializeToBytes() const {
  return this->pm->SerializeAsString();
}

HandshakeInitMessage::~HandshakeInitMessage() {}
}
