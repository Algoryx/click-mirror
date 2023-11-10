#include <memory>
#include <Messaging.pb.h>
#include <click/SensorMessageBuilder.h>

using namespace click;
using namespace std;


SensorMessageBuilderImpl::SensorMessageBuilderImpl(unique_ptr<protobuf::SensorMessage> control_m)
{
    this->message = std::move(control_m);
}

AddSensorValuesBuilder *SensorMessageBuilderImpl::object(const string& name)
{
    google::protobuf::Map<string, protobuf::SensorMessage_Object> *map = this->message->mutable_objects();
    // Allocate new object
    map->operator[](name);
    currObject = &(*map)[name];
    return this;
}

AddSensorValuesBuilder *SensorMessageBuilderImpl::withAngles(const vector<double>& angles)
{
    currObject->mutable_anglesensors()->Assign(angles.begin(), angles.end());
    return this;
}

AddSensorValuesBuilder *SensorMessageBuilderImpl::withPosition(const Vec3& vec3)
{
    auto object_sensor = currObject->add_objectsensors();
    object_sensor->mutable_position()->mutable_arr()->Assign(vec3.begin(), vec3.end());
    return this;
}

AddSensorValuesBuilder *SensorMessageBuilderImpl::withRPY(const Vec3& vec3)
{
    auto object_sensor = currObject->add_objectsensors();
    object_sensor->mutable_rpy()->mutable_arr()->Assign(vec3.begin(), vec3.end());
    return this;
}

CLICK_EXPORT AddSensorBuilder *click::SensorMessageBuilderImpl::withSensor(const std::string &name)
{
    protobuf::SensorMessage_Sensors sensor;
    (*currObject->mutable_sensors())[name] = sensor;
    curr_sensor = &(*currObject->mutable_sensors())[name];
    return this;
}

CLICK_EXPORT AddSensorBuilder *click::SensorMessageBuilderImpl::withAngle(double angle)
{
    auto sensor = curr_sensor->add_sensor();
    sensor->set_angle(angle);
    return this;
}

CLICK_EXPORT AddSensorBuilder *click::SensorMessageBuilderImpl::withAngularVelocity(double angular_velocity)
{
    auto sensor = curr_sensor->add_sensor();
    sensor->set_anglevelocity(angular_velocity);
    return this;
}

CLICK_EXPORT AddSensorBuilder *click::SensorMessageBuilderImpl::withTorque(double torque)
{
    auto sensor = curr_sensor->add_sensor();
    sensor->set_torque(torque);
    return this;
}

CLICK_EXPORT AddSensorBuilder *click::SensorMessageBuilderImpl::withPosition_(const Vec3 &vec3)
{
    auto sensor = curr_sensor->add_sensor();
    sensor->mutable_position()->mutable_arr()->Assign(vec3.begin(), vec3.end());
    return this;
}

CLICK_EXPORT AddSensorBuilder *click::SensorMessageBuilderImpl::withRPY_(const Vec3 &vec3)
{
    auto sensor = curr_sensor->add_sensor();
    sensor->mutable_rpy()->mutable_arr()->Assign(vec3.begin(), vec3.end());
    return this;
}

CLICK_EXPORT AddSensorBuilder *click::SensorMessageBuilderImpl::withActivated(bool activated)
{
    auto sensor = curr_sensor->add_sensor();
    sensor->set_activated(activated);
    return this;
}

CLICK_EXPORT AddSensorBuilder *click::SensorMessageBuilderImpl::withAcceleration(const Vec3 &vec3)
{
    auto sensor = curr_sensor->add_sensor();
    sensor->mutable_acceleration()->mutable_arr()->Assign(vec3.begin(), vec3.end());
    return this;
}

CLICK_EXPORT AddSensorBuilder *click::SensorMessageBuilderImpl::withForce(const Vec3 &vec3)
{
    auto sensor = curr_sensor->add_sensor();
    sensor->mutable_force()->mutable_arr()->Assign(vec3.begin(), vec3.end());
    return this;
}

CLICK_EXPORT AddSensorBuilder *click::SensorMessageBuilderImpl::withDirectionalTorque(const Vec3 &vec3)
{
    auto sensor = curr_sensor->add_sensor();
    sensor->mutable_directionaltorque()->mutable_arr()->Assign(vec3.begin(), vec3.end());
    return this;
}

CLICK_EXPORT AddSensorBuilder *click::SensorMessageBuilderImpl::withAngularAcceleration(const Vec3 &vec3)
{
    auto sensor = curr_sensor->add_sensor();
    sensor->mutable_angularacceleration()->mutable_arr()->Assign(vec3.begin(), vec3.end());
    return this;
}

AddSensorValuesBuilder *SensorMessageBuilderImpl::withAngularVelocities(const vector<double> &angles)
{
    currObject->mutable_anglevelocitysensors()->Assign(angles.begin(), angles.end());
    return this;
}

AddSensorValuesBuilder *SensorMessageBuilderImpl::withTorques(const vector<double>& torques)
{
    currObject->mutable_torquesensors()->Assign(torques.begin(), torques.end());
    return this;
}

unique_ptr<SensorMessage> SensorMessageBuilderImpl::build()
{
    return unique_ptr<SensorMessage>(new SensorMessage(std::move(message)));
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
