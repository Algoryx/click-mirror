#include <memory>
#include <Messaging.pb.h>
#include <click/SensorMessageBuilder.h>
#include <click/SensorMessage.h>

using namespace click;
using namespace std;


SensorMessageBuilderImpl::SensorMessageBuilderImpl(unique_ptr<protobuf::SensorMessage> control_m)
{
    this->m_message = std::move(control_m);
}

AddSensorValuesBuilder *SensorMessageBuilderImpl::object(const string& name)
{
    google::protobuf::Map<string, protobuf::SensorMessage_Object> *map = this->m_message->mutable_objects();
    // Allocate new object
    map->operator[](name);
    m_curr_object = &(*map)[name];
    return this;
}

AddSensorValuesBuilder *click::SensorMessageBuilderImpl::withSimulatedTime(double time)
{
    m_message->mutable_simvars()->set_simulatedtime(time);
    return this;
}

AddSensorValuesBuilder *SensorMessageBuilderImpl::withAngles(const vector<double> &angles)
{
    m_curr_object->mutable_anglesensors()->Assign(angles.begin(), angles.end());
    return this;
}

AddSensorValuesBuilder *SensorMessageBuilderImpl::withPosition(const Vec3& vec3)
{
    auto object_sensor = m_curr_object->add_objectsensors();
    object_sensor->mutable_position()->mutable_arr()->Assign(vec3.begin(), vec3.end());
    return this;
}

AddSensorValuesBuilder *SensorMessageBuilderImpl::withRPY(const Vec3& vec3)
{
    auto object_sensor = m_curr_object->add_objectsensors();
    object_sensor->mutable_rpy()->mutable_arr()->Assign(vec3.begin(), vec3.end());
    return this;
}

AddSensorBuilder *click::SensorMessageBuilderImpl::withSensor(const std::string& name)
{
    protobuf::SensorMessage_Sensors sensor;
    (*m_curr_object->mutable_sensors())[name] = sensor;
    m_curr_sensor = &(*m_curr_object->mutable_sensors())[name];
    return this;
}

AddSensorBuilder *click::SensorMessageBuilderImpl::withAngle(double angle)
{
    auto sensor = m_curr_sensor->add_sensor();
    sensor->set_angle(angle);
    return this;
}

AddSensorBuilder *click::SensorMessageBuilderImpl::withAngularVelocity1d(double angular_velocity)
{
    auto sensor = m_curr_sensor->add_sensor();
    sensor->set_angularvelocity1d(angular_velocity);
    return this;
}

AddSensorBuilder *click::SensorMessageBuilderImpl::withTorque1d(double torque)
{
    auto sensor = m_curr_sensor->add_sensor();
    sensor->set_torque1d(torque);
    return this;
}

AddSensorBuilder *click::SensorMessageBuilderImpl::withPosition_(const Vec3& vec3)
{
    auto sensor = m_curr_sensor->add_sensor();
    sensor->mutable_position()->mutable_arr()->Assign(vec3.begin(), vec3.end());
    return this;
}

AddSensorBuilder *click::SensorMessageBuilderImpl::withRPY_(const Vec3& vec3)
{
    auto sensor = m_curr_sensor->add_sensor();
    sensor->mutable_rpy()->mutable_arr()->Assign(vec3.begin(), vec3.end());
    return this;
}

AddSensorBuilder *click::SensorMessageBuilderImpl::withActivated(bool activated)
{
    auto sensor = m_curr_sensor->add_sensor();
    sensor->set_activated(activated);
    return this;
}

AddSensorBuilder *click::SensorMessageBuilderImpl::withAcceleration3d(const Vec3& vec3)
{
    auto sensor = m_curr_sensor->add_sensor();
    sensor->mutable_acceleration3d()->mutable_arr()->Assign(vec3.begin(), vec3.end());
    return this;
}

AddSensorBuilder *click::SensorMessageBuilderImpl::withForce3d(const Vec3& vec3)
{
    auto sensor = m_curr_sensor->add_sensor();
    sensor->mutable_force3d()->mutable_arr()->Assign(vec3.begin(), vec3.end());
    return this;
}

AddSensorBuilder *click::SensorMessageBuilderImpl::withTorque3d(const Vec3& vec3)
{
    auto sensor = m_curr_sensor->add_sensor();
    sensor->mutable_torque3d()->mutable_arr()->Assign(vec3.begin(), vec3.end());
    return this;
}

AddSensorBuilder *click::SensorMessageBuilderImpl::withAngularAcceleration3d(const Vec3& vec3)
{
    auto sensor = m_curr_sensor->add_sensor();
    sensor->mutable_angularacceleration3d()->mutable_arr()->Assign(vec3.begin(), vec3.end());
    return this;
}

AddSensorBuilder *click::SensorMessageBuilderImpl::withVelocity3d(const Vec3 &vec3)
{
    auto sensor = m_curr_sensor->add_sensor();
    sensor->mutable_velocity3d()->mutable_arr()->Assign(vec3.begin(), vec3.end());
    return this;
}

AddSensorBuilder *click::SensorMessageBuilderImpl::withAngularVelocity3d(const Vec3 &vec3)
{
    auto sensor = m_curr_sensor->add_sensor();
    sensor->mutable_angularvelocity3d()->mutable_arr()->Assign(vec3.begin(), vec3.end());
    return this;
}

AddSensorValuesBuilder *SensorMessageBuilderImpl::withAngularVelocities(const vector<double> &angles)
{
    m_curr_object->mutable_anglevelocitysensors()->Assign(angles.begin(), angles.end());
    return this;
}

AddSensorValuesBuilder *SensorMessageBuilderImpl::withTorques(const vector<double>& torques)
{
    m_curr_object->mutable_torquesensors()->Assign(torques.begin(), torques.end());
    return this;
}

unique_ptr<SensorMessage> SensorMessageBuilderImpl::build()
{
    return unique_ptr<SensorMessage>(new SensorMessage(std::move(m_message)));
}

unique_ptr<SensorMessageBuilder> SensorMessageBuilderImpl::builder()
{
    unique_ptr<protobuf::SensorMessage> control_m = make_unique<protobuf::SensorMessage>();
    control_m->set_messagetype(protobuf::SensorMessageType);
    return unique_ptr<SensorMessageBuilder>(new SensorMessageBuilderImpl(std::move(control_m)));
}

SensorMessageBuilder::~SensorMessageBuilder() = default;
SensorMessageBuilderImpl::~SensorMessageBuilderImpl() = default;
AddSensorValuesBuilder::~AddSensorValuesBuilder() = default;
