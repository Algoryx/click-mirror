#include <Messaging.pb.h>
#include <click/ControlMessage.h>
#include <click/HandshakeMessage.h>
#include <click/HandshakeInitMessage.h>
#include <click/ErrorMessage.h>
#include <click/MessageSerializer.h>
#include <click/ResetMessage.h>
#include <click/SensorMessage.h>
#include <click/SensorRequestMessage.h>

using namespace click;
using namespace std;

string MessageSerializer::serializeToString(const Message& message)
{
  return message.serializeToBytes();
}

unique_ptr<Message> MessageSerializer::sensorMessageFromIStream(istream *input)
{
  unique_ptr<protobuf::SensorMessage> pm = make_unique<protobuf::SensorMessage>();
  pm->ParseFromIstream(input);
  // Need to explicitly call private constructor
  return unique_ptr<SensorMessage>(new SensorMessage(std::move(pm)));
}

unique_ptr<Message> MessageSerializer::handshakeMessageFromIStream(istream *input)
{
  unique_ptr<protobuf::HandshakeMessage> pm = make_unique<protobuf::HandshakeMessage>();
  pm->ParseFromIstream(input);
  // Need to explicitly call private constructor
  return unique_ptr<HandshakeMessage>(new HandshakeMessage(std::move(pm)));
}

unique_ptr<Message> MessageSerializer::fromBytes(const string& bytes)
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
      return unique_ptr<HandshakeInitMessage>(new HandshakeInitMessage(std::move(pm)));
    }
    case protobuf::ErrorMessageType:
    {
      unique_ptr<protobuf::ErrorMessage> pm = make_unique<protobuf::ErrorMessage>();
      pm->ParseFromString(bytes);
      // Need to explicitly call private constructor
      return unique_ptr<ErrorMessage>(new ErrorMessage(std::move(pm)));
    }
    case protobuf::ResetMessageType:
    {
      unique_ptr<protobuf::ResetMessage> pm = make_unique<protobuf::ResetMessage>();
      pm->ParseFromString(bytes);
      // Need to explicitly call private constructor
      return unique_ptr<ResetMessage>(new ResetMessage(std::move(pm)));
    }
    case protobuf::SensorRequestMessageType:
    {
      unique_ptr<protobuf::SensorRequestMessage> pm = make_unique<protobuf::SensorRequestMessage>();
      pm->ParseFromString(bytes);
      // Need to explicitly call private constructor
      return unique_ptr<SensorRequestMessage>(new SensorRequestMessage(std::move(pm)));
    }
    case protobuf::HandshakeMessageType:
    {
      unique_ptr<protobuf::HandshakeMessage> pm = make_unique<protobuf::HandshakeMessage>();
      pm->ParseFromString(bytes);
      // Need to explicitly call private constructor
      return unique_ptr<HandshakeMessage>(new HandshakeMessage(std::move(pm)));
    }
    case protobuf::ControlMessageType:
    {
      unique_ptr<protobuf::ControlMessage> pm = make_unique<protobuf::ControlMessage>();
      pm->ParseFromString(bytes);
      // Need to explicitly call private constructor
      return unique_ptr<ControlMessage>(new ControlMessage(std::move(pm)));
    }
    case protobuf::SensorMessageType:
    {
      unique_ptr<protobuf::SensorMessage> pm = make_unique<protobuf::SensorMessage>();
      pm->ParseFromString(bytes);
      // Need to explicitly call private constructor
      return unique_ptr<SensorMessage>(new SensorMessage(std::move(pm)));
    }
    default:
      throw runtime_error("Serializing not implemented for: " + pm.DebugString());
  }
}
