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

    class CLICK_EXPORT ControlMessageBuilder
    {
    public:
        /**
         * Add an additional object to Message
         *
         * \return a Builder
         */
        virtual AddControlBuilder *object(std::string name) = 0;
        virtual ~ControlMessageBuilder();
    };

    class CLICK_EXPORT AddControlBuilder
    {
    public:
        /**
         * Add per joint values to the current object
         * This method can always be used in place of withAngles, withAngleVelocities, or withTorques
         *
         * \return a Builder
         */
        virtual AddControlEventBuilder *withValues(std::vector<double> values) = 0;
        /**
         * @deprecated use withValues
         * Add angles to the current object
         * Can only be used when all Robot joints have the same input signal type
         *
         * \return a Builder
         */
        virtual AddControlEventBuilder *withAngles(std::vector<double> angles) = 0;
        /**
         * @deprecated use withValues
         * Add angleVelocities to the current object
         *
         * \return a Builder
         */
        virtual AddControlEventBuilder *withAngleVelocities(std::vector<double> angles) = 0;
        /**
         * @deprecated use withValues
         * Add torques to the current object
         * Can only be used when all Robot joints have the same input signal type
         *
         * \return a Builder
         */
        virtual AddControlEventBuilder *withTorques(std::vector<double> torques) = 0;
        /**
         * Build the message
         * Can only be used when all Robot joints have the same input signal type
         *
         * \return a ControlMessage
         */
        virtual std::unique_ptr<ControlMessage> build() = 0;
        virtual ~AddControlBuilder();
    };

    class CLICK_EXPORT AddControlEventBuilder
    {
    public:
        /**
         * Add a control event to current object
         *
         * \return a Builder
         */
        virtual AddControlEventBuilder *withControlEvent(std::string name, bool activated) = 0;
        /**
         * Add an additional object to Message
         *
         * \return a Builder
         */
        virtual AddControlBuilder *object(std::string name) = 0;
        /**
         * Build the message
         *
         * \return a ControlMessage
         */
        virtual std::unique_ptr<ControlMessage> build() = 0;
        virtual ~AddControlEventBuilder();
    };

    class CLICK_EXPORT ControlMessageBuilderImpl : ControlMessageBuilder, AddControlBuilder, AddControlEventBuilder
    {
    public:
        virtual AddControlBuilder *object(std::string name);
        virtual AddControlEventBuilder *withAngles(std::vector<double> angles);
        virtual AddControlEventBuilder *withValues(std::vector<double> values);
        virtual AddControlEventBuilder *withAngleVelocities(std::vector<double> angles);
        virtual AddControlEventBuilder *withTorques(std::vector<double> torques);
        virtual AddControlEventBuilder *withControlEvent(std::string name, bool activated);
        virtual std::unique_ptr<ControlMessage> build();
        virtual ~ControlMessageBuilderImpl();
        /**
         * Create a builder that creates a Message
         *
         * \return a Builder
         */
        static std::unique_ptr<ControlMessageBuilder> builder();

    private:
        ControlMessageBuilderImpl(std::unique_ptr<protobuf::ControlMessage> control_m);
        std::unique_ptr<protobuf::ControlMessage> message;
        protobuf::ControlMessage_Object *currObject;
    };
}
