#include<string>
#include<memory>
#include <click/ControlMessage.h>

class algoryx::click::ControlMessageBuilder {

public:
    ControlMessageBuilder *object(std::string name);
    ControlMessageBuilder *withAngles(std::vector<double> angles);
    ControlMessageBuilder *withAngleVelocities(std::vector<double> angles);
    ControlMessageBuilder *withTorques(std::vector<double> torques);
    ControlMessageBuilder *withControlEvent(std::string name, bool activated);
    std::unique_ptr<ControlMessage> build();
    static std::unique_ptr<ControlMessageBuilder> builder();

private:
    ControlMessageBuilder(std::unique_ptr<protobuf::ControlMessage> control_m);
    std::unique_ptr<protobuf::ControlMessage> message;
    protobuf::ControlMessage_Object *currObject;
};
