#pragma once
#include <string>
#include <memory>
#include <click/DllExport.h>
#include <click/ControlMessage.h>

namespace click
{
    namespace protobuf
    {
        class ControlMessage_Object;
    }

    class AddControlBuilder;
    class AddControlEventBuilder;

    class ControlMessageBuilder
    {
    public:
        CLICK_EXPORT virtual AddControlBuilder *object(std::string name) = 0;
        CLICK_EXPORT virtual ~ControlMessageBuilder();
    };

    class AddControlBuilder
    {
    public:
        CLICK_EXPORT virtual AddControlEventBuilder *withAngles(std::vector<double> angles) = 0;
        CLICK_EXPORT virtual AddControlEventBuilder *withAngleVelocities(std::vector<double> angles) = 0;
        CLICK_EXPORT virtual AddControlEventBuilder *withTorques(std::vector<double> torques) = 0;
        CLICK_EXPORT virtual std::unique_ptr<ControlMessage> build() = 0;
        CLICK_EXPORT virtual ~AddControlBuilder();
    };

    class AddControlEventBuilder
    {
    public:
        CLICK_EXPORT virtual ControlMessageBuilder *withControlEvent(std::string name, bool activated) = 0;
        CLICK_EXPORT virtual AddControlBuilder *object(std::string name) = 0;
        CLICK_EXPORT virtual std::unique_ptr<ControlMessage> build() = 0;
        CLICK_EXPORT virtual ~AddControlEventBuilder();
    };

    class ControlMessageBuilderImpl : ControlMessageBuilder, AddControlBuilder, AddControlEventBuilder
    {
    public:
        CLICK_EXPORT virtual AddControlBuilder *object(std::string name);
        CLICK_EXPORT virtual AddControlEventBuilder *withAngles(std::vector<double> angles);
        CLICK_EXPORT virtual AddControlEventBuilder *withAngleVelocities(std::vector<double> angles);
        CLICK_EXPORT virtual AddControlEventBuilder *withTorques(std::vector<double> torques);
        CLICK_EXPORT virtual ControlMessageBuilder *withControlEvent(std::string name, bool activated);
        CLICK_EXPORT virtual std::unique_ptr<ControlMessage> build();
        CLICK_EXPORT virtual ~ControlMessageBuilderImpl();
        CLICK_EXPORT static std::unique_ptr<ControlMessageBuilder> builder();

    private:
        ControlMessageBuilderImpl(std::unique_ptr<protobuf::ControlMessage> control_m);
        std::unique_ptr<protobuf::ControlMessage> message;
        protobuf::ControlMessage_Object *currObject;
    };
}
