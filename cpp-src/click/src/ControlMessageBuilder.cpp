#include <memory>
#include <click/ControlMessageBuilder.h>

using namespace algoryx::click;
using namespace std;

ControlMessageBuilder::ControlMessageBuilder(unique_ptr<protobuf::ControlMessage> control_m)
{
    this->message = move(control_m);
}

ControlMessageBuilder *ControlMessageBuilder::object(string name)
{
    google::protobuf::Map<string, protobuf::ControlMessage_Object> *map = this->message->mutable_objects();
    // Allocate new object
    map->operator[](name);
    currObject = &(*map)[name];
    return this;
}
ControlMessageBuilder *ControlMessageBuilder::withAngles(vector<double> angles)
{
    currObject->mutable_angles()->Assign(angles.begin(), angles.end());
    return this;
}
ControlMessageBuilder *ControlMessageBuilder::withAngleVelocities(vector<double> angles)
{
    currObject->mutable_anglevelocities()->Assign(angles.begin(), angles.end());
    return this;
}
ControlMessageBuilder *ControlMessageBuilder::withTorques(vector<double> torques)
{
    currObject->mutable_torques()->Assign(torques.begin(), torques.end());
    return this;
}
ControlMessageBuilder *ControlMessageBuilder::withControlEvent(string name, bool activated)
{
    (*currObject->mutable_controlevents())["gripper"] = true;
    return this;
}
unique_ptr<ControlMessage> ControlMessageBuilder::build()
{
    return unique_ptr<ControlMessage>(new ControlMessage(move(message)));
}

unique_ptr<ControlMessageBuilder> ControlMessageBuilder::builder()
{
    unique_ptr<protobuf::ControlMessage> control_m = make_unique<protobuf::ControlMessage>();
    control_m->set_messagetype(protobuf::ControlMessageType);
    return unique_ptr<ControlMessageBuilder>(new ControlMessageBuilder(move(control_m)));
}
