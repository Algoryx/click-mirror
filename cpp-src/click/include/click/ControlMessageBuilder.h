#pragma once
#include <string>
#include <memory>
#include <click/ControlMessage.h>

namespace click { namespace protobuf {
  class ControlMessage_Object;
}}

namespace click {

    class AddControlBuilder;
    class AddControlEventBuilder;

    class ControlMessageBuilder
    {
        public:
            virtual AddControlBuilder *object(std::string name) = 0;
            virtual ~ControlMessageBuilder();
    };

    class AddControlBuilder
    {
    public:
        virtual AddControlEventBuilder *withAngles(std::vector<double> angles) = 0;
        virtual AddControlEventBuilder *withAngleVelocities(std::vector<double> angles) = 0;
        virtual AddControlEventBuilder *withTorques(std::vector<double> torques) = 0;
        virtual std::unique_ptr<ControlMessage> build() = 0;
        virtual ~AddControlBuilder();
    };

    class AddControlEventBuilder
    {
    public:
        virtual ControlMessageBuilder *withControlEvent(std::string name, bool activated) = 0;
        virtual AddControlBuilder *object(std::string name) = 0;
        virtual std::unique_ptr<ControlMessage> build() = 0;
        virtual ~AddControlEventBuilder();
    };

    class ControlMessageBuilderImpl: ControlMessageBuilder, AddControlBuilder, AddControlEventBuilder
    {
    public:
        virtual AddControlBuilder *object(std::string name);
        virtual AddControlEventBuilder *withAngles(std::vector<double> angles);
        virtual AddControlEventBuilder *withAngleVelocities(std::vector<double> angles);
        virtual AddControlEventBuilder *withTorques(std::vector<double> torques);
        virtual ControlMessageBuilder *withControlEvent(std::string name, bool activated);
        virtual std::unique_ptr<ControlMessage> build();
        virtual ~ControlMessageBuilderImpl();
        static std::unique_ptr<ControlMessageBuilder> builder();

    private:
        ControlMessageBuilderImpl(std::unique_ptr<protobuf::ControlMessage> control_m);
        std::unique_ptr<protobuf::ControlMessage> message;
        protobuf::ControlMessage_Object *currObject;
    };
}
