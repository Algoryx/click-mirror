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
  throw runtime_error("RPY not found in " + this->debugString());
}

// TODO: Add hasObjectPosition or return bool, take vector
// Vec3 && SensorMessage::objectPosition(const string &objectname, const vector<double> &target) const {

// TODO: Test && as retval!
vector<double> SensorMessage::objectPosition(const string &objectname) const {
  for (auto &sensor : this->sensorMess->objects().at(objectname).objectsensors())
    if (sensor.has_position()) {
      auto vec3 = sensor.position();
      return vector<double>{vec3.arr().begin(), vec3.arr().end()};
    }
  throw runtime_error("Position not found in " + this->debugString());
}

inline void copy_n(const protobuf::SensorMessage_Vec3 &src, Vec3 &trg) {
  copy_n(src.arr().begin(), 3, trg.begin());
}

vector<Sensor> SensorMessage::sensor(const string &objectname, const string &sensorname) const
{
  vector<Sensor> res(this->sensorMess->objects().at(objectname).sensors().at(sensorname).sensor().size());
  auto target = res.begin();
  for (auto &sensor : this->sensorMess->objects().at(objectname).sensors().at(sensorname).sensor()) {
    if (sensor.has_acceleration())
      copy_n(sensor.acceleration(), target->acceleration);
    else if (sensor.has_activated())
      target->activated = sensor.activated();
    else if (sensor.has_angle())
      target->angle = sensor.angle();
    else if (sensor.has_anglevelocity())
      target->angleVelocity = sensor.anglevelocity();
    else if (sensor.has_angularacceleration())
      copy_n(sensor.angularacceleration(), target->angularAcceleration);
    else if (sensor.has_directionaltorque())
      copy_n(sensor.directionaltorque(), target->directionalTorque);
    else if (sensor.has_force())
      copy_n(sensor.force(), target->force);
    else if (sensor.has_position())
      copy_n(sensor.position(), target->position);
    else if (sensor.has_rpy())
      copy_n(sensor.rpy(), target->rpy);
    else if (sensor.has_torque())
      target->torque = sensor.torque();
    else
      throw runtime_error("Return not implemented for " + sensor.DebugString());
    target++;
  }
  return res;
}

MessageType SensorMessage::messageType() const {
  return static_cast<MessageType>(sensorMess->messagetype());
}

string SensorMessage::serializeToBytes() const {
  return this->sensorMess->SerializeAsString();
}

SensorMessage::~SensorMessage() = default;
