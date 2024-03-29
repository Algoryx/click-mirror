#include <Messaging.pb.h>
#include <click/HandshakeMessage.h>

using namespace click;
using namespace std;


vector<string> HandshakeMessage::objects() const {
  vector<string> objects;

  for (auto& pair : this->pm->objects())
    objects.push_back(pair.first);
  return objects;
}

vector<string> HandshakeMessage::controlsInOrder(const std::string& objectname) const{
  auto vec = this->pm->objects().at(objectname).controlsinorder();
  return vector<string>(vec.begin(), vec.end());
}

vector<ValueType> HandshakeMessage::controlTypesInOrder(const std::string& objectname) const{
  vector<ValueType> controlTypes;
  for (auto& sensor : this->pm->objects().at(objectname).controltypesinorder())
    controlTypes.push_back(static_cast<ValueType>(sensor));
  return controlTypes;
}

vector<string> HandshakeMessage::jointSensorsInOrder(const std::string& objectname) const{
  auto vec = this->pm->objects().at(objectname).jointsensorsinorder();
  return vector<string>(vec.begin(), vec.end());
}

vector<ValueType> HandshakeMessage::jointSensors(const std::string& objectname) const
{
  vector<ValueType> sensors;
  for (auto& sensor : this->pm->objects().at(objectname).jointsensors())
    sensors.push_back(static_cast<ValueType>(sensor));
  return sensors;
}

vector<ValueType> HandshakeMessage::sensors(const std::string& objectname, const std::string& sensorname) const
{
  vector<ValueType> sensors;
  for (auto& sensor : this->pm->objects().at(objectname).sensors().at(sensorname).types())
    sensors.push_back(static_cast<ValueType>(sensor));
  return sensors;
}

vector<ValueType> HandshakeMessage::objectSensors(const std::string& objectname) const
{
  vector<ValueType> sensors;
  for (auto& sensor : this->pm->objects().at(objectname).objectsensors())
      sensors.push_back(static_cast<ValueType>(sensor));
  return sensors;
}

vector<string> HandshakeMessage::controlEvents(const std::string& objectname) const
{
  vector<string> sensors;
  for (auto& pair : this->pm->objects().at(objectname).controlevents())
      sensors.push_back(pair.first);
  return sensors;
}

ValueType HandshakeMessage::controlEvent(const std::string& objectname, const std::string& eventname) const
{
  auto sensor = this->pm->objects().at(objectname).controlevents().at(eventname);
  return static_cast<ValueType>(sensor);
}

HandshakeMessage::SimulationSettings HandshakeMessage::simulationSettings() const
{
  return {this->pm->simsettings().timestep()};
}

ValueType HandshakeMessage::controlType() const
{
  return static_cast<ValueType>(this->pm->controltype());
}

HandshakeMessage::HandshakeMessage(unique_ptr<protobuf::HandshakeMessage> HandshakeMessage)
{
  this->pm = std::move(HandshakeMessage);
};

string HandshakeMessage::debugString() const
{
  return this->pm->DebugString();
}

MessageType HandshakeMessage::messageType() const {
  return static_cast<MessageType>(pm->messagetype());
}

string HandshakeMessage::serializeToBytes() const {
  return this->pm->SerializeAsString();
}

HandshakeMessage::~HandshakeMessage() = default;

unique_ptr<HandshakeMessage> click::toHandshakeMessage(unique_ptr<Message> message) {
    return unique_ptr<HandshakeMessage>(static_cast<HandshakeMessage *>(message.release()));
}

