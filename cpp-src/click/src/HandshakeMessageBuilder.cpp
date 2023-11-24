#include <memory>
#include <Messaging.pb.h>
#include <click/HandshakeMessageBuilder.h>
#include <click/HandshakeMessageBuilder.h>

using namespace click;
using namespace std;

HandshakeMessageBuilderImpl::HandshakeMessageBuilderImpl(unique_ptr<protobuf::HandshakeMessage> pm)
{
    m_message = std::move(pm);
}

unique_ptr<HandshakeMessage> HandshakeMessageBuilderImpl::build()
{
    return unique_ptr<HandshakeMessage>(new HandshakeMessage(std::move(m_message)));
}

unique_ptr<HandshakeMessageBuilder> HandshakeMessageBuilderImpl::builder()
{
    unique_ptr<protobuf::HandshakeMessage> pm = make_unique<protobuf::HandshakeMessage>();
    pm->set_messagetype(protobuf::HandshakeMessageType);
    pm->set_version(protobuf::CURRENT_VERSION);
    return unique_ptr<HandshakeMessageBuilderImpl>(new HandshakeMessageBuilderImpl(std::move(pm)));
}

CLICK_EXPORT HandshakeMessageBuilder *
click::HandshakeMessageBuilderImpl::withSimulationSettings(const click::HandshakeMessage::SimulationSettings &simulation_settings)
{
    m_message->mutable_simsettings()->set_timestep(simulation_settings.timeStep);
    return this;
}

CLICK_EXPORT HandshakeMessageBuilder *click::HandshakeMessageBuilderImpl::withControlType(const click::ValueType &control_type)
{
    protobuf::ValueType control_type_ = (protobuf::ValueType) control_type;
    m_message->set_controltype(control_type_);
    return this;
}

CLICK_EXPORT AddRobotBuilder *click::HandshakeMessageBuilderImpl::withRobot(const std::string &name)
{
    auto * map = m_message->mutable_objects();
    // Allocate new object
    // (*map)[name];
    m_curr_object = &(*map)[name];
    return this;
}

CLICK_EXPORT AddRobotBuilder *click::HandshakeMessageBuilderImpl::withControlsInOrder(const std::vector<std::string> &order)
{
    m_curr_object->mutable_controlsinorder()->Assign(order.begin(), order.end());
    return this;
}

CLICK_EXPORT AddRobotBuilder *click::HandshakeMessageBuilderImpl::withControlTypesInOrder(const std::vector<click::ValueType> &order)
{
    for (auto & item : order) {
        m_curr_object->mutable_controltypesinorder()->Add((protobuf::ValueType) item);
    }
    return this;
}

CLICK_EXPORT AddRobotBuilder *click::HandshakeMessageBuilderImpl::withJointSensorsInOrder(const std::vector<std::string> &order)
{
    m_curr_object->mutable_jointsensorsinorder()->Assign(order.begin(), order.end());
    return this;
}

CLICK_EXPORT AddRobotBuilder *click::HandshakeMessageBuilderImpl::withJointSensors(const std::vector<click::ValueType> &order)
{
    for (auto & item : order) {
        m_curr_object->mutable_jointsensors()->Add((protobuf::ValueType) item);
    }
    return this;
}

CLICK_EXPORT AddHandshakeSensorBuilder *click::HandshakeMessageBuilderImpl::withSensor(const std::string &name)
{
    auto * map = m_curr_object->mutable_sensors();
    m_curr_sensor = &(*map)[name];
    return this;
}

CLICK_EXPORT AddRobotBuilder *click::HandshakeMessageBuilderImpl::withControlEvent(const std::string &name, const click::ValueType &type)
{
    auto * value_ref = &(*m_curr_object->mutable_controlevents())[name];
    *value_ref = (protobuf::ValueType) type;
    return this;
}

CLICK_EXPORT AddRobotBuilder *click::HandshakeMessageBuilderImpl::withObjectSensors(std::vector<click::ValueType> &order)
{
    for (auto & item : order) {
        m_curr_object->mutable_objectsensors()->Add((protobuf::ValueType) item);
    }
    return this;
}

CLICK_EXPORT AddRobotBuilder *click::HandshakeMessageBuilderImpl::withTypesInOrder(const std::vector<click::ValueType> &order)
{
    for (auto & item : order) {
        m_curr_sensor->mutable_types()->Add((protobuf::ValueType) item);
    }
    return this;
}

HandshakeMessageBuilderImpl::~HandshakeMessageBuilderImpl() = default;
click::AddRobotBuilder::~AddRobotBuilder() = default;
click::AddHandshakeSensorBuilder::~AddHandshakeSensorBuilder() = default;
HandshakeMessageBuilder::~HandshakeMessageBuilder() = default;
