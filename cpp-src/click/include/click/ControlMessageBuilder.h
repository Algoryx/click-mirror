#pragma once
#include <string>
#include <memory>
#include <click/ControlMessage.h>

namespace algoryx { namespace click {

    class AddControlBuilder;
    class AddControlEventBuilder;

    class ControlMessageBuilder
    {
        public:
            virtual AddControlBuilder *object(std::string name) = 0;
            virtual ~ControlMessageBuilder() = default;
    };

    class AddControlBuilder
    {
    public:
        virtual AddControlEventBuilder *withAngles(std::vector<double> angles) = 0;
        virtual AddControlEventBuilder *withAngleVelocities(std::vector<double> angles) = 0;
        virtual AddControlEventBuilder *withTorques(std::vector<double> torques) = 0;
        virtual std::unique_ptr<ControlMessage> build() = 0;
        virtual ~AddControlBuilder() = default;
    };

    class AddControlEventBuilder
    {
    public:
        virtual ControlMessageBuilder *withControlEvent(std::string name, bool activated) = 0;
        virtual AddControlBuilder *object(std::string name) = 0;
        virtual std::unique_ptr<ControlMessage> build() = 0;
        virtual ~AddControlEventBuilder() = default;
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
        virtual ~ControlMessageBuilderImpl() = default;
        static std::unique_ptr<ControlMessageBuilder> builder();

    private:
        ControlMessageBuilderImpl(std::unique_ptr<protobuf::ControlMessage> control_m);
        std::unique_ptr<protobuf::ControlMessage> message;
        protobuf::ControlMessage_Object *currObject;
    };
}}
