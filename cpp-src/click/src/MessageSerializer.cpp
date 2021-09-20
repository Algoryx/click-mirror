#include <click/MessageSerializer.h>
#include <click/ControlMessage.h>
#include <click/SensorMessage.h>

using namespace algoryx::click;


std::string MessageSerializer::serializeToString(const Message& message)
{
  return message.serializeToBytes();
}

Message * MessageSerializer::fromIStream(std::istream *input) {
  // protobuf::Message message;
  // message.ParseFromIstream(input);
  // switch(message.messagetype()) {
  //   case protobuf::ControlMessageType:
  //   {
    //   protobuf::ControlMessage * cm = new protobuf::ControlMessage();
    //   cm->ParseFromIstream(input);
    //   return new ControlMessage(cm);
    // }
    // case protobuf::SensorMessageType:
    // {
      protobuf::SensorMessage * cm = new protobuf::SensorMessage();
      cm->ParseFromIstream(input);
      return new SensorMessage(cm);
  //   }
  //   default:
  //   {
  //     Message * bad;
  //     bad->debugString();
  //     return bad;
  //   }
  // }

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
      bad->debugString();
      return bad;
    }
  }
}
