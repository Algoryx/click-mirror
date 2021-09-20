#include <click/MessageSerializer.h>
#include <click/ControlMessage.h>

using namespace algoryx::click;


std::string MessageSerializer::serializeToString(const Message& message)
{
  return message.serializeToBytes();
}

Message * MessageSerializer::fromBytes(const std::string &bytes)
{
  protobuf::Message message;
  message.ParseFromString(bytes);
  switch(message.messagetype()) {
    case protobuf::ControlMessageType:
    {
      protobuf::ControlMessage * cm = new protobuf::ControlMessage();
      cm->ParseFromString(bytes);
      return new ControlMessage(cm);
    }
    default:
    {
      Message * bad;
      bad->DebugString();
      return bad;
    }
  }
}
