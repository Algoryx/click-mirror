#include <memory>
#include <Messaging.pb.h>
#include <click/ControlMessageBuilder.h>

using namespace click;
using namespace std;


ControlMessageBuilderImpl::ControlMessageBuilderImpl(unique_ptr<protobuf::ControlMessage> control_m)
{
    this->message = std::move(control_m);
}

AddControlBuilder *ControlMessageBuilderImpl::object(string name)
{
    google::protobuf::Map<string, protobuf::ControlMessage_Object> *map = this->message->mutable_objects();
    // Allocate new object
    map->operator[](name);
    currObject = &(*map)[name];
    return this;
}
AddControlEventBuilder *ControlMessageBuilderImpl::withAngles(vector<double> angles)
{
    currObject->mutable_angles()->Assign(angles.begin(), angles.end());
    return this;
}
AddControlEventBuilder *ControlMessageBuilderImpl::withValues(vector<double> values)
{
    currObject->mutable_values()->Assign(values.begin(), values.end());
    return this;
}
AddControlEventBuilder *ControlMessageBuilderImpl::withAngularVelocities(vector<double> angles)
{
    currObject->mutable_angularvelocities()->Assign(angles.begin(), angles.end());
    return this;
}
AddControlEventBuilder *ControlMessageBuilderImpl::withTorques(vector<double> torques)
{
    currObject->mutable_torques()->Assign(torques.begin(), torques.end());
    return this;
}
AddControlEventBuilder *ControlMessageBuilderImpl::withControlEvent(string name, bool activated)
{
    (*currObject->mutable_controlevents())[name] = activated;
    return this;
}
unique_ptr<ControlMessage> ControlMessageBuilderImpl::build()
{
    return unique_ptr<ControlMessage>(new ControlMessage(std::move(message)));
}

unique_ptr<ControlMessageBuilder> ControlMessageBuilderImpl::builder()
{
    unique_ptr<protobuf::ControlMessage> control_m = make_unique<protobuf::ControlMessage>();
    control_m->set_messagetype(protobuf::ControlMessageType);
    return unique_ptr<ControlMessageBuilder>(new ControlMessageBuilderImpl(std::move(control_m)));
}

ControlMessageBuilder::~ControlMessageBuilder() = default;
AddControlBuilder::~AddControlBuilder() = default;
AddControlEventBuilder::~AddControlEventBuilder() = default;
ControlMessageBuilderImpl::~ControlMessageBuilderImpl() = default;
