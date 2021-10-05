#include <memory>
#include <Messaging.pb.h>
#include <click/ControlMessageBuilder.h>

using namespace click;
using namespace std;


CLICK_EXPORT ControlMessageBuilderImpl::ControlMessageBuilderImpl(unique_ptr<protobuf::ControlMessage> control_m)
{
    this->message = move(control_m);
}

CLICK_EXPORT AddControlBuilder *ControlMessageBuilderImpl::object(string name)
{
    google::protobuf::Map<string, protobuf::ControlMessage_Object> *map = this->message->mutable_objects();
    // Allocate new object
    map->operator[](name);
    currObject = &(*map)[name];
    return this;
}
CLICK_EXPORT AddControlEventBuilder *ControlMessageBuilderImpl::withAngles(vector<double> angles)
{
    currObject->mutable_angles()->Assign(angles.begin(), angles.end());
    return this;
}
CLICK_EXPORT AddControlEventBuilder *ControlMessageBuilderImpl::withAngleVelocities(vector<double> angles)
{
    currObject->mutable_anglevelocities()->Assign(angles.begin(), angles.end());
    return this;
}
CLICK_EXPORT AddControlEventBuilder *ControlMessageBuilderImpl::withTorques(vector<double> torques)
{
    currObject->mutable_torques()->Assign(torques.begin(), torques.end());
    return this;
}
CLICK_EXPORT ControlMessageBuilder *ControlMessageBuilderImpl::withControlEvent(string name, bool activated)
{
    (*currObject->mutable_controlevents())["gripper"] = true;
    return this;
}
CLICK_EXPORT unique_ptr<ControlMessage> ControlMessageBuilderImpl::build()
{
    return unique_ptr<ControlMessage>(new ControlMessage(move(message)));
}

CLICK_EXPORT unique_ptr<ControlMessageBuilder> ControlMessageBuilderImpl::builder()
{
    unique_ptr<protobuf::ControlMessage> control_m = make_unique<protobuf::ControlMessage>();
    control_m->set_messagetype(protobuf::ControlMessageType);
    return unique_ptr<ControlMessageBuilder>(new ControlMessageBuilderImpl(move(control_m)));
}

CLICK_EXPORT ControlMessageBuilder::~ControlMessageBuilder() = default;
CLICK_EXPORT AddControlBuilder::~AddControlBuilder() = default;
CLICK_EXPORT AddControlEventBuilder::~AddControlEventBuilder() = default;
CLICK_EXPORT ControlMessageBuilderImpl::~ControlMessageBuilderImpl() = default;
