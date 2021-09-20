#include <click/SensorMessage.h>

using namespace algoryx::click;

SensorMessage::SensorMessage(algoryx::click::protobuf::SensorMessage * sensor_m)
{
  this->sensor_m = sensor_m;
};

std::string SensorMessage::debugString() const
{
  return this->sensor_m->DebugString();
}

// TODO: Fill provided vector instead of returning?
std::vector<double> SensorMessage::angles(const std::string &objectname) const {
  int len = this->sensor_m->objects().at(objectname).sensors().size();
  std::vector<double> res;
  // TODO: XXX Hard time parsing stdalone if nameorder not passed here, prefer 0-7!
  for(int i=0; i<len; i++)
    res.push_back(this->sensor_m->objects().at(objectname).sensors().at("joint1").sensor().at(0).angle());
  return res;
}

// TODO: How reduce duplication? (Note, we might change this protocol - fix later)
std::vector<double> SensorMessage::angleVelocities(const std::string &objectname) const {
  int len = this->sensor_m->objects().at(objectname).sensors().size();
  std::vector<double> res;
  for(int i=0; i<len; i++)
    res.push_back(this->sensor_m->objects().at(objectname).sensors().at("joint1").sensor().at(1).anglevelocity());
  return res;
}

std::vector<double> SensorMessage::torques(const std::string &objectname) const {
  int len = this->sensor_m->objects().at(objectname).sensors().size();
  std::vector<double> res;
  for(int i=0; i<len; i++)
    res.push_back(this->sensor_m->objects().at(objectname).sensors().at("joint1").sensor().at(2).torque());
  return res;
}

std::vector<double> SensorMessage::objectRPY(const std::string &objectname, int index) const {
  int len = this->sensor_m->objects().at(objectname).objectsensors().size();
  if (this->sensor_m->objects().at(objectname).objectsensors().at(0).has_rpy()) {
    auto vec = this->sensor_m->objects().at(objectname).objectsensors().at(0).rpy().arr();
    std::vector<double> res(vec.begin(), vec.end());
    return res;
  }
  return std::vector<double>(); // TODO: throw exception.
}

// bool SensorMessage::sensorEvent(const std::string &objectname, std::string sensorname) const {
//   return this->sensor_m->objects().at(objectname).sensorevents().at(sensorname);
// }

// TODO: move to Message?
MessageType SensorMessage::messageType() const {
  return static_cast<MessageType>(sensor_m->messagetype());
}

std::string SensorMessage::serializeToBytes() const {
  return this->sensor_m->SerializeAsString();
}

SensorMessage::~SensorMessage() {
  // TODO: How test that sensor_m is deallocated?
  delete sensor_m;
}
