#include <Messaging.pb.h>
#include <click/SensorMessage.h>
#include <iostream>

using namespace click;

SensorMessage::SensorMessage(std::unique_ptr<protobuf::SensorMessage> sensorMessage)
{
  this->sensorMess = std::move(sensorMessage);
};

std::vector<double> SensorMessage::angles(const std::string &objectname) const
{

  auto vec = this->sensorMess->objects().at(objectname).anglesensors();
  return std::vector<double>(vec.begin(), vec.end());
}

std::vector<double> SensorMessage::angleVelocities(const std::string &objectname) const
{
  auto vec = this->sensorMess->objects().at(objectname).anglevelocitysensors();
  return std::vector<double>(vec.begin(), vec.end());
}

std::string SensorMessage::debugString() const
{
  return this->sensorMess->DebugString();
}

MessageType SensorMessage::messageType() const
{
  return static_cast<MessageType>(sensorMess->messagetype());
}

Vec3 SensorMessage::objectRPY(const std::string &objectname) const
{
  for (auto &sensor : this->sensorMess->objects().at(objectname).objectsensors())
    if (sensor.has_rpy())
    {
      auto vec = sensor.rpy();
      return Vec3{vec.arr().at(0), vec.arr().at(1), vec.arr().at(2)};
    }
  throw std::runtime_error("RPY not found in " + this->debugString());
}

// Vec3 will RVO:d according to https://stackoverflow.com/questions/27368236/return-value-or-rvalue-reference
Vec3 SensorMessage::objectPosition(const std::string &objectname) const
{
  for (auto &sensor : this->sensorMess->objects().at(objectname).objectsensors())
    if (sensor.has_position())
    {
      auto vec3 = sensor.position();
      return std::move(Vec3{vec3.arr().at(0), vec3.arr().at(1), vec3.arr().at(2)});
    }
  throw std::runtime_error("Position not found in " + this->debugString());
}

inline void copy_n(const protobuf::SensorMessage_Vec3 &src, Vec3 &trg)
{
  std::copy_n(src.arr().begin(), 3, trg.begin());
}

inline Vec3 createFrom(const protobuf::SensorMessage_Vec3 &src)
{
  return Vec3{src.arr().at(0), src.arr().at(1), src.arr().at(2)};
}

std::vector<Sensor> SensorMessage::sensor(const std::string &objectname, const std::string &sensorname) const
{
  std::vector<Sensor> res(this->sensorMess->objects().at(objectname).sensors().at(sensorname).sensor().size());
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
      throw std::runtime_error("Return not implemented for " + sensor.DebugString());
    target++;
  }
  return res;
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
    throw std::runtime_error("Not a Vec3: " + sensor.DebugString());
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
    throw std::runtime_error("Not a double: " + sensor.DebugString());
}

bool SensorMessage::sensorBool(const std::string &objectname, const std::string &sensorname, int idx) const
{
  auto &sensor = this->sensorMess->objects().at(objectname).sensors().at(sensorname).sensor().at(idx);
  if (sensor.has_activated())
    return sensor.activated();
  else
    throw std::runtime_error("Not a bool: " + sensor.DebugString());
}

double SensorMessage::simulatedTime() const
{
  return this->sensorMess->simvars().simulatedtime();
}

std::vector<double> SensorMessage::torques(const std::string &objectname) const
{
  auto vec = this->sensorMess->objects().at(objectname).torquesensors();
  return std::vector<double>(vec.begin(), vec.end());
}

/*
 * Privates
 */

std::string SensorMessage::serializeToBytes() const
{
  return this->sensorMess->SerializeAsString();
}

SensorMessage::~SensorMessage() = default;

std::unique_ptr<SensorMessage> click::toSensorMessage(std::unique_ptr<Message> message)
{
  return std::unique_ptr<SensorMessage>(static_cast<SensorMessage *>(message.release()));
}

