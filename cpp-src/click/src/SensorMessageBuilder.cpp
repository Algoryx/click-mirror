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

AddSensorValuesBuilder *SensorMessageBuilderImpl::withAngleVelocities(const vector<double>& angles)
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
