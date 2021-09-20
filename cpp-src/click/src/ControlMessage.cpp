#include <click/ControlMessage.h>

namespace algoryx { namespace click {

ControlMessage::ControlMessage(algoryx::click::protobuf::ControlMessage * control_m)
{
  this->control_m = control_m;
};

std::string ControlMessage::debugString() const
{
  return this->control_m->DebugString();
}

std::vector<double> ControlMessage::angles(const std::string &objectname) const {
  google::protobuf::RepeatedField<double> arr = this->control_m->objects().at(objectname).angles();
  return std::vector<double>(arr.begin(), arr.end());
}

std::vector<double> ControlMessage::angleVelocities(const std::string &objectname) const {
  google::protobuf::RepeatedField<double> arr = this->control_m->objects().at(objectname).anglevelocities();
  return std::vector<double>(arr.begin(), arr.end());
}

std::vector<double> ControlMessage::torques(const std::string &objectname) const {
  google::protobuf::RepeatedField<double> arr = this->control_m->objects().at(objectname).torques();
  return std::vector<double>(arr.begin(), arr.end());
}

bool ControlMessage::controlEvent(const std::string &objectname, std::string controlname) const {
  return this->control_m->objects().at(objectname).controlevents().at(controlname);
}


MessageType ControlMessage::messageType() const {
  return static_cast<MessageType>(control_m->messagetype());
}

std::string ControlMessage::serializeToBytes() const {
  return this->control_m->SerializeAsString();
}

ControlMessage::~ControlMessage() {
  // TODO: How test that control_m is deallocated?
  delete control_m;
}
}}
