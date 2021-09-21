#include <click/SensorMessage.h>
#include<iostream>

using namespace algoryx::click;
using namespace std;

unique_ptr<SensorMessage> algoryx::click::toSensorMessage(unique_ptr<Message> message) {
    return unique_ptr<SensorMessage>(static_cast<SensorMessage *>(message.release()));
}

SensorMessage::SensorMessage(unique_ptr<protobuf::SensorMessage> sensorMessage)
{
  this->sensorMess = move(sensorMessage);
};

string SensorMessage::debugString() const
{
  return this->sensorMess->DebugString();
}

// TODO: Fill provided vector instead of returning?
vector<double> SensorMessage::angles(const string &objectname) const {

  auto vec = this->sensorMess->objects().at(objectname).anglesensors();
  return vector<double>(vec.begin(), vec.end());
}

vector<double> SensorMessage::angleVelocities(const string &objectname) const
{
  auto vec = this->sensorMess->objects().at(objectname).anglevelocitysensors();
  return vector<double>(vec.begin(), vec.end());
}

vector<double> SensorMessage::torques(const string &objectname) const {
  auto vec = this->sensorMess->objects().at(objectname).torquesensors();
  return vector<double>(vec.begin(), vec.end());
}

vector<double> SensorMessage::objectRPY(const string &objectname) const {
  for (auto &sensor : this->sensorMess->objects().at(objectname).objectsensors())
    if (sensor.has_rpy())
    {
      auto vec = sensor.rpy();
      return vector<double>(vec.arr().begin(), vec.arr().end());
    }
  throw std::runtime_error("RPY not found in " + this->debugString());
}

// TODO: Add hasObjectPosition or return bool, take vector
vector<double> SensorMessage::objectPosition(const string &objectname) const {
  for (auto &sensor : this->sensorMess->objects().at(objectname).objectsensors())
    if (sensor.has_position()) {
      auto vec3 = sensor.position();
      return vector<double>{vec3.x(), vec3.y(), vec3.z()};
    }
  throw std::runtime_error("Position not found in " + this->debugString());
}

MessageType SensorMessage::messageType() const {
  return static_cast<MessageType>(sensorMess->messagetype());
}

string SensorMessage::serializeToBytes() const {
  return this->sensorMess->SerializeAsString();
}

SensorMessage::~SensorMessage() = default;
