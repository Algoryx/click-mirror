#include <click/ControlMessage.h>

namespace algoryx { namespace click {

ControlMessage::ControlMessage(algoryx::click::protobuf::ControlMessage * control_m)
{
  this->control_m = control_m;
};

std::string ControlMessage::DebugString() const
{
  return this->control_m->DebugString();
}

std::vector<double> double_vector_with(std::initializer_list<double> doubles) {
    return std::vector<double>(doubles);
}

std::vector<double> ControlMessage::angles(std::string objectname) const {
  google::protobuf::RepeatedField<double> arr = this->control_m->objects().at(objectname).angles();
  return std::vector(arr.begin(), arr.end());
}
std::vector<double> ControlMessage::angleVelocities(std::string objectname) const {
  google::protobuf::RepeatedField<double> arr = this->control_m->objects().at(objectname).anglevelocities();
  return std::vector(arr.begin(), arr.end());
}
std::vector<double> ControlMessage::torques(std::string objectname) const {
  google::protobuf::RepeatedField<double> arr = this->control_m->objects().at(objectname).torques();
  return std::vector(arr.begin(), arr.end());
}
bool ControlMessage::controlEvent(std::string objectname, std::string controlname) const {
  return this->control_m->objects().at(objectname).controlevents().at(controlname);
}


MessageType ControlMessage::messageType() const {
  return static_cast<MessageType>(control_m->messagetype());
}


ControlMessage::~ControlMessage() = default;
}}


/*
void ControlMessage::parseFromString(const std::string& data)
{
  auto signal = Grpc::ControlSignals();
  signal.ParseFromString(data);
  m_jointTorques.clear();
  for (auto&& torque : signal.jointtorques())
  {
    m_jointTorques.push_back(torque);
  }
  m_triggerStep = signal.triggerstep();
}

std::string ControlMessage::serializeAsString() const
{
  auto signal = Grpc::ControlSignals();
  signal.set_type(Grpc::MessageType::ControlSignalType);
  for (auto&& torque : m_jointTorques)
  {
    signal.add_jointtorques(torque);
  }
  signal.set_triggerstep(m_triggerStep);

  return signal.SerializeAsString();
}

std::vector<double> ControlMessage::getJointTorques() const
{
  return m_jointTorques;
}

bool ControlMessage::getTriggerStep() const
{
  return m_triggerStep;
}

void ControlMessage::addJointTorque(double torque)
{
  m_jointTorques.push_back(torque);
}

void ControlMessage::setJointTorques(std::vector<double>& torques)
{
  m_jointTorques.assign(torques.begin(), torques.end());
}

void ControlMessage::setTriggerStep(bool enable)
{
  m_triggerStep = enable;
  //m_signal->set_triggerstep(enable);
}

*/

