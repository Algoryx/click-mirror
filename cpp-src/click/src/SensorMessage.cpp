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
  int len = this->sensorMess->objects().at(objectname).sensors().size();
  vector<double> res;
  // TODO: XXX Hard time parsing stdalone if nameorder not passed here, prefer 0-7!
  for(int i=0; i<len; i++)
    res.push_back(this->sensorMess->objects().at(objectname).sensors().at("joint1").sensor().at(0).angle());
  return res;
}

// TODO: How reduce duplication? (Note, we might change this protocol - fix later)
vector<double> SensorMessage::angleVelocities(const string &objectname) const {
  int len = this->sensorMess->objects().at(objectname).sensors().size();
  vector<double> res;
  for(int i=0; i<len; i++)
    res.push_back(this->sensorMess->objects().at(objectname).sensors().at("joint1").sensor().at(1).anglevelocity());
  return res;
}

vector<double> SensorMessage::torques(const string &objectname) const {
  int len = this->sensorMess->objects().at(objectname).sensors().size();
  vector<double> res;
  for(int i=0; i<len; i++)
    res.push_back(this->sensorMess->objects().at(objectname).sensors().at("joint1").sensor().at(2).torque());
  return res;
}

vector<double> SensorMessage::objectRPY(const string &objectname) const {
  for (auto sensor : this->sensorMess->objects().at(objectname).objectsensors())
    if (sensor.has_rpy())
    {
      auto vec = sensor.rpy();
      return vector<double>(vec.arr().begin(), vec.arr().end());
    }
  throw std::runtime_error("RPY not found in " + this->debugString());
}

// TODO: Add hasObjectPosition or return bool, take vector
vector<double> SensorMessage::objectPosition(const string &objectname) const {
  for (auto sensor : this->sensorMess->objects().at(objectname).objectsensors())
    if (sensor.has_position()) {
      auto vec3 = sensor.position();
      return vector<double>{vec3.x(), vec3.y(), vec3.z()};
    }
  throw std::runtime_error("Position not found in " + this->debugString());
}

// TODO: Implement sensorevent!
// bool SensorMessage::sensorEvent(const string &objectname, string sensorname) const {
//   return this->sensor_m->objects().at(objectname).sensorevents().at(sensorname);
// }

MessageType SensorMessage::messageType() const {
  return static_cast<MessageType>(sensorMess->messagetype());
}

string SensorMessage::serializeToBytes() const {
  return this->sensorMess->SerializeAsString();
}

SensorMessage::~SensorMessage() = default;
