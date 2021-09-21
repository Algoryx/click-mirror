#include <click/MessageSerializer.h>
#include <click/ControlMessage.h>
#include <click/SensorMessage.h>

using namespace algoryx::click;
using namespace std;


string MessageSerializer::serializeToString(const Message& message)
{
  return message.serializeToBytes();
}

unique_ptr<Message> MessageSerializer::sensorMessageFromIStream(istream *input) {
  // Is it possible to read from stream twice to facilitate switch?
  unique_ptr<protobuf::SensorMessage> pm = make_unique<protobuf::SensorMessage>();
  pm->ParseFromIstream(input);
  // Need to explicitly call private constructor
  return unique_ptr<SensorMessage>(new SensorMessage(move(pm)));
}

unique_ptr<Message> MessageSerializer::fromBytes(const string &bytes)
{
  protobuf::Message pm;
  pm.ParseFromString(bytes);
  switch(pm.messagetype()) {
    case protobuf::ControlMessageType:
    {
      unique_ptr<protobuf::ControlMessage> cm = make_unique<protobuf::ControlMessage>();
      cm->ParseFromString(bytes);
      // Need to explicitly call private constructor
      return unique_ptr<ControlMessage>(new ControlMessage(move(cm)));
    }
    default:
      
      throw runtime_error("Serializing not implemented for: " + pm.DebugString());
  }
}
