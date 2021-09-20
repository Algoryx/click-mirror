#include <click/MessageSerializer.h>
#include <click/Message.h>
#include <click/ControlMessage.h>
#include <click/SensorMessage.h>
#include <sstream>

using namespace algoryx::click;


std::string MessageSerializer::serializeToString(const Message& message)
{
  return message.serializeToBytes();
}

/*
MessageSerializer::~MessageSerializer()
{
}

bool MessageSerializer::decodeMessage(const std::string& data, Message& message)
{
  auto grpcMessage = Grpc::BaseMessage();
  grpcMessage.ParseFromString(data);
  switch (grpcMessage.type())
  {
    case Grpc::MessageType::ControlSignalType:
      return MessageSerializer::decodeControlMessage(data, message);
    case Grpc::MessageType::SensorSignalType:
      return MessageSerializer::decodeSensorMessage(data, message);
    default:
      std::stringstream errorString;
      errorString << "Unsupported message type when decoding message: " << grpcMessage.type();
      throw std::runtime_error(errorString.str());
    }
}

std::string MessageSerializer::encodeMessage(const SensorMessage& message)
{
  auto signal = Grpc::SensorSignals();
  signal.set_type(Grpc::MessageType::SensorSignalType);
  for (auto&& angle : message.getJointAngles())
    signal.add_jointangles(angle);
  return signal.SerializeAsString();
}

bool MessageSerializer::decodeControlMessage(const std::string& data, Message& outMessage)
{
  auto grpcMessage = Grpc::ControlSignals();
  grpcMessage.ParseFromString(data);
  auto* controlMessage = dynamic_cast<ControlMessage*>(&outMessage);
  if (controlMessage == nullptr)
  {
    std::cerr << "Invalid message type passed to decodeControlMessage. Aborting decoding." << std::endl;
    return false;
  }
  for (auto&& torque : grpcMessage.jointtorques())
    controlMessage->addJointTorque(torque);
  controlMessage->setTriggerStep(grpcMessage.triggerstep());
  return true;
}

bool MessageSerializer::decodeSensorMessage(const std::string& data, Message& outMessage)
{
  auto grpcMessage = Grpc::SensorSignals();
  grpcMessage.ParseFromString(data);
  auto* sensorMessage = dynamic_cast<SensorMessage*>(&outMessage);
  if (sensorMessage == nullptr)
  {
    std::cerr << "Invalid message type passed to decodeSensorMessage. Aborting decoding." << std::endl;
    return false;
  }
  for (auto&& angle : grpcMessage.jointangles())
    sensorMessage->addJointAngle(angle);
  return true;
}
*/
