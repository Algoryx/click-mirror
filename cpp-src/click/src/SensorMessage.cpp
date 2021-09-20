#include <click/SensorMessage.h>


RCP::SensorMessage::SensorMessage()
{
  //m_signal = std::make_unique<Rcp::Grpc::SensorSignals>();
  //m_signal->set_type(Rcp::Grpc::MessageType::SensorSignalType);
}

void RCP::SensorMessage::parseFromString(const std::string& data)
{
  auto signal = Rcp::Grpc::SensorSignals();
  signal.ParseFromString(data);
  //for (int i = 0; i < signal.jointangles_size(); i++)
  m_jointAngles.clear();
  for (auto&& angle : signal.jointangles())
  {
    m_jointAngles.push_back(angle);
  }
}

std::string RCP::SensorMessage::serializeAsString() const
{
  auto signal = Rcp::Grpc::SensorSignals();
  signal.set_type(Rcp::Grpc::MessageType::SensorSignalType);
  for (auto&& angle : m_jointAngles)
  {
    signal.add_jointangles(angle);
  }
  return signal.SerializeAsString();
}

std::vector<double> RCP::SensorMessage::getJointAngles() const
{
  return m_jointAngles;
}

void RCP::SensorMessage::addJointAngle(double angle)
{
  m_jointAngles.push_back(angle);
}

//std::string RCP::SensorMessage::getDebugString() const
//{
//  return m_signal->DebugString();
//}

RCP::SensorMessage::~SensorMessage() = default;
