#include <Messaging.pb.h>
#include <click/SensorMessage.h>
#include <iostream>

using namespace click;

SensorMessage::SensorMessage(std::unique_ptr<protobuf::SensorMessage> sensorMessage)
{
  this->sensorMess = std::move(sensorMessage);
};

std::vector<double> SensorMessage::angles(const std::string& objectname) const
{

  auto vec = this->sensorMess->objects().at(objectname).anglesensors();
  return std::vector<double>(vec.begin(), vec.end());
}

std::vector<double> SensorMessage::angularVelocities(const std::string& objectname) const
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

Vec3 SensorMessage::objectRPY(const std::string& objectname) const
{
  for (auto& sensor : this->sensorMess->objects().at(objectname).objectsensors())
    if (sensor.has_rpy())
    {
      auto vec = sensor.rpy();
      return Vec3{vec.arr().at(0), vec.arr().at(1), vec.arr().at(2)};
    }
  throw std::runtime_error("RPY not found in " + this->debugString());
}

// Vec3 will RVO:d according to https://stackoverflow.com/questions/27368236/return-value-or-rvalue-reference
Vec3 SensorMessage::objectPosition(const std::string& objectname) const
{
  for (auto& sensor : this->sensorMess->objects().at(objectname).objectsensors())
    if (sensor.has_position())
    {
      auto vec3 = sensor.position();
      return std::move(Vec3{vec3.arr().at(0), vec3.arr().at(1), vec3.arr().at(2)});
    }
  throw std::runtime_error("Position not found in " + this->debugString());
}

inline void copy_n(const protobuf::SensorMessage_Vec3& src, Vec3& trg)
{
  std::copy_n(src.arr().begin(), 3, trg.begin());
}

inline Vec3 createFrom(const protobuf::SensorMessage_Vec3& src)
{
  return Vec3{src.arr().at(0), src.arr().at(1), src.arr().at(2)};
}

std::vector<Sensor> SensorMessage::sensor(const std::string& objectname, const std::string& sensorname) const
{
  std::vector<Sensor> res(this->sensorMess->objects().at(objectname).sensors().at(sensorname).sensor().size());
  auto target = res.begin();
  for (auto& sensor : this->sensorMess->objects().at(objectname).sensors().at(sensorname).sensor())
  {
    if (sensor.has_acceleration3d()) {
      copy_n(sensor.acceleration3d(), target->value.acceleration3d);
      target->type = click::ValueType::Acceleration3D;
    }
    else if (sensor.has_activated()) {
      target->value.activated = sensor.activated();
      target->type = click::ValueType::Activated;
    }
    else if (sensor.has_angle()) {
      target->value.angle = sensor.angle();
      target->type = click::ValueType::Angle;
    }
    else if (sensor.has_angularvelocity1d()) {
      target->value.angularVelocity1d = sensor.angularvelocity1d();
      target->type = click::ValueType::AngularVelocity1D;
    }
    else if (sensor.has_angularacceleration3d()) {
      copy_n(sensor.angularacceleration3d(), target->value.angularAcceleration3d);
      target->type = click::ValueType::AngularAcceleration3D;
    }
    else if (sensor.has_torque3d()) {
      copy_n(sensor.torque3d(), target->value.torque3d);
      target->type = click::ValueType::Torque3D;
    }
    else if (sensor.has_force3d()) {
      copy_n(sensor.force3d(), target->value.force3d);
      target->type = click::ValueType::Force3D;
    }
    else if (sensor.has_position()) {
      copy_n(sensor.position(), target->value.position);
      target->type = click::ValueType::Position;
    }
    else if (sensor.has_rpy()) {
      copy_n(sensor.rpy(), target->value.rpy);
      target->type = click::ValueType::RPY;
    }
    else if (sensor.has_torque1d()) {
      target->value.torque1d = sensor.torque1d();
      target->type = click::ValueType::Torque1D;
    }
    else if (sensor.has_velocity3d()) {
      copy_n(sensor.velocity3d(), target->value.velocity3d);
      target->type = click::ValueType::Velocity3D;
    }
    else if (sensor.has_angularvelocity3d()) {
      copy_n(sensor.angularvelocity3d(), target->value.angularVelocity3d);
      target->type = click::ValueType::AngularVelocity3D;
    }
    else
      throw std::runtime_error("Return not implemented for " + sensor.DebugString());
    target++;
  }
  return res;
}

Vec3 SensorMessage::sensorVec3(const std::string& objectname, const std::string& sensorname, int idx) const
{
  auto& sensor = this->sensorMess->objects().at(objectname).sensors().at(sensorname).sensor().at(idx);
  if (sensor.has_acceleration3d())
    return createFrom(sensor.acceleration3d());
  else if (sensor.has_angularacceleration3d())
    return createFrom(sensor.angularacceleration3d());
  else if (sensor.has_torque3d())
    return createFrom(sensor.torque3d());
  else if (sensor.has_force3d())
    return createFrom(sensor.force3d());
  else if (sensor.has_position())
    return createFrom(sensor.position());
  else if (sensor.has_rpy())
    return createFrom(sensor.rpy());
  else
    throw std::runtime_error("Not a Vec3: " + sensor.DebugString());
}

double SensorMessage::sensorDouble(const std::string& objectname, const std::string& sensorname, int idx) const
{
  auto& sensor = this->sensorMess->objects().at(objectname).sensors().at(sensorname).sensor().at(idx);
  if (sensor.has_angle())
    return sensor.angle();
  else if (sensor.has_angularvelocity1d())
    return sensor.angularvelocity1d();
  else if (sensor.has_torque1d())
    return sensor.torque1d();
  else
    throw std::runtime_error("Not a double: " + sensor.DebugString());
}

bool SensorMessage::sensorBool(const std::string& objectname, const std::string& sensorname, int idx) const
{
  auto& sensor = this->sensorMess->objects().at(objectname).sensors().at(sensorname).sensor().at(idx);
  if (sensor.has_activated())
    return sensor.activated();
  else
    throw std::runtime_error("Not a bool: " + sensor.DebugString());
}

double SensorMessage::simulatedTime() const
{
  return this->sensorMess->simvars().simulatedtime();
}

std::vector<double> SensorMessage::torques(const std::string& objectname) const
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

