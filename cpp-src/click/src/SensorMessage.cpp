#include <Messaging.pb.h>
#include <click/SensorMessage.h>
#include <iostream>

using namespace click;
using namespace std;

SensorMessage::SensorMessage(unique_ptr<protobuf::SensorMessage> sensorMessage)
{
  this->sensorMess = move(sensorMessage);
};

string SensorMessage::debugString() const
{
  return this->sensorMess->DebugString();
}

vector<double> SensorMessage::angles(const string &objectname) const
{

  auto vec = this->sensorMess->objects().at(objectname).anglesensors();
  return vector<double>(vec.begin(), vec.end());
}

vector<double> SensorMessage::angleVelocities(const string &objectname) const
{
  auto vec = this->sensorMess->objects().at(objectname).anglevelocitysensors();
  return vector<double>(vec.begin(), vec.end());
}

vector<double> SensorMessage::torques(const string &objectname) const
{
  auto vec = this->sensorMess->objects().at(objectname).torquesensors();
  return vector<double>(vec.begin(), vec.end());
}

Vec3 SensorMessage::objectRPY(const string &objectname) const
{
  for (auto &sensor : this->sensorMess->objects().at(objectname).objectsensors())
    if (sensor.has_rpy())
    {
      auto vec = sensor.rpy();
      return Vec3{vec.arr().at(0), vec.arr().at(1), vec.arr().at(2)};
    }
  throw runtime_error("RPY not found in " + this->debugString());
}

// Vec3 will RVO:d according to https://stackoverflow.com/questions/27368236/return-value-or-rvalue-reference
Vec3 SensorMessage::objectPosition(const string &objectname) const
{
  for (auto &sensor : this->sensorMess->objects().at(objectname).objectsensors())
    if (sensor.has_position())
    {
      auto vec3 = sensor.position();
      return move(Vec3{vec3.arr().at(0), vec3.arr().at(1), vec3.arr().at(2)});
    }
  throw runtime_error("Position not found in " + this->debugString());
}

inline void copy_n(const protobuf::SensorMessage_Vec3 &src, Vec3 &trg)
{
  copy_n(src.arr().begin(), 3, trg.begin());
}

inline Vec3 createFrom(const protobuf::SensorMessage_Vec3 &src)
{
  return Vec3{src.arr().at(0), src.arr().at(1), src.arr().at(2)};
}

Vec3 SensorMessage::sensorVec3(const std::string &objectname, const std::string &sensorname, int idx) const
{
  auto &sensor = this->sensorMess->objects().at(objectname).sensors().at(sensorname).sensor().at(idx);
  if (sensor.has_acceleration())
    return createFrom(sensor.acceleration());
  else if (sensor.has_angularacceleration())
    return createFrom(sensor.angularacceleration());
  else if (sensor.has_directionaltorque())
    return createFrom(sensor.directionaltorque());
  else if (sensor.has_force())
    return createFrom(sensor.force());
  else if (sensor.has_position())
    return createFrom(sensor.position());
  else if (sensor.has_rpy())
    return createFrom(sensor.rpy());
  else
    throw runtime_error("Not a Vec3: " + sensor.DebugString());
}

double SensorMessage::sensorDouble(const std::string &objectname, const std::string &sensorname, int idx) const
{
  auto &sensor = this->sensorMess->objects().at(objectname).sensors().at(sensorname).sensor().at(idx);
  if (sensor.has_angle())
    return sensor.angle();
  else if (sensor.has_anglevelocity())
    return sensor.anglevelocity();
  else if (sensor.has_torque())
    return sensor.torque();
  else
    throw runtime_error("Not a double: " + sensor.DebugString());
}

bool SensorMessage::sensorBool(const std::string &objectname, const std::string &sensorname, int idx) const
{
  auto &sensor = this->sensorMess->objects().at(objectname).sensors().at(sensorname).sensor().at(idx);
  if (sensor.has_activated())
    return sensor.activated();
  else
    throw runtime_error("Not a bool: " + sensor.DebugString());
}

vector<Sensor> SensorMessage::sensor(const string &objectname, const string &sensorname) const
{
  vector<Sensor> res(this->sensorMess->objects().at(objectname).sensors().at(sensorname).sensor().size());
  auto target = res.begin();
  for (auto &sensor : this->sensorMess->objects().at(objectname).sensors().at(sensorname).sensor())
  {
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

MessageType SensorMessage::messageType() const
{
  return static_cast<MessageType>(sensorMess->messagetype());
}

string SensorMessage::serializeToBytes() const
{
  return this->sensorMess->SerializeAsString();
}

SensorMessage::~SensorMessage() = default;

unique_ptr<SensorMessage> click::toSensorMessage(unique_ptr<Message> message)
{
  return unique_ptr<SensorMessage>(static_cast<SensorMessage *>(message.release()));
}
