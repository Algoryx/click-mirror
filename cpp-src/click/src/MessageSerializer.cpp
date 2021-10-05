#include <Messaging.pb.h>
#include <click/MessageSerializer.h>
#include <click/ControlMessage.h>
#include <click/HandshakeMessage.h>
#include <click/HandshakeInitMessage.h>
#include <click/SensorMessage.h>

using namespace click;
using namespace std;

string MessageSerializer::serializeToString(const Message &message)
{
  return message.serializeToBytes();
}

unique_ptr<Message> MessageSerializer::sensorMessageFromIStream(istream *input)
{
  unique_ptr<protobuf::SensorMessage> pm = make_unique<protobuf::SensorMessage>();
  pm->ParseFromIstream(input);
  // Need to explicitly call private constructor
  return unique_ptr<SensorMessage>(new SensorMessage(move(pm)));
}

unique_ptr<Message> MessageSerializer::handshakeMessageFromIStream(istream *input)
{
  unique_ptr<protobuf::HandshakeMessage> pm = make_unique<protobuf::HandshakeMessage>();
  pm->ParseFromIstream(input);
  // Need to explicitly call private constructor
  return unique_ptr<HandshakeMessage>(new HandshakeMessage(move(pm)));
}

unique_ptr<Message> MessageSerializer::fromBytes(const string &bytes)
{
  protobuf::Message pm;
  pm.ParseFromString(bytes);
  switch (pm.messagetype())
  {
  case protobuf::HandshakeInitMessageType:
  {
    unique_ptr<protobuf::HandshakeInitMessage> pm = make_unique<protobuf::HandshakeInitMessage>();
    pm->ParseFromString(bytes);
    // Need to explicitly call private constructor
    return unique_ptr<HandshakeInitMessage>(new HandshakeInitMessage(move(pm)));
  }
  case protobuf::HandshakeMessageType:
  {
    unique_ptr<protobuf::HandshakeMessage> pm = make_unique<protobuf::HandshakeMessage>();
    pm->ParseFromString(bytes);
    // Need to explicitly call private constructor
    return unique_ptr<HandshakeMessage>(new HandshakeMessage(move(pm)));
  }
  case protobuf::ControlMessageType:
  {
    unique_ptr<protobuf::ControlMessage> pm = make_unique<protobuf::ControlMessage>();
    pm->ParseFromString(bytes);
    // Need to explicitly call private constructor
    return unique_ptr<ControlMessage>(new ControlMessage(move(pm)));
  }
  case protobuf::SensorMessageType:
  {
    unique_ptr<protobuf::SensorMessage> pm = make_unique<protobuf::SensorMessage>();
    pm->ParseFromString(bytes);
    // Need to explicitly call private constructor
    return unique_ptr<SensorMessage>(new SensorMessage(move(pm)));
  }
  default:

    throw runtime_error("Serializing not implemented for: " + pm.DebugString());
  }
}
