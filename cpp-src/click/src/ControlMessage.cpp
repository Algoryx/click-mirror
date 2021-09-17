#include <click/ControlMessage.h>

using namespace algoryx::click;

ControlMessage::ControlMessage()
{
};

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

//std::string ControlMessage::getDebugString() const
//{
//  return m_signal->DebugString();
//}

ControlMessage::~ControlMessage() = default;
