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
        /**
         * Add an additional object to Message
         *
         * \return a Builder
         */
        CLICK_EXPORT virtual AddControlBuilder *object(std::string name) = 0;
        CLICK_EXPORT virtual ~ControlMessageBuilder();
    };

    class AddControlBuilder
    {
    public:
        /**
         * Add per joint values to the current object
         * This method can always be used in place of withAngles, withAngleVelocities, or withTorques
         *
         * \return a Builder
         */
        CLICK_EXPORT virtual AddControlEventBuilder *withValues(std::vector<double> values) = 0;
        /**
         * @deprecated use withValues
         * Add angles to the current object
         * Can only be used when all Robot joints have the same input signal type
         *
         * \return a Builder
         */
        CLICK_EXPORT virtual AddControlEventBuilder *withAngles(std::vector<double> angles) = 0;
        /**
         * @deprecated use withValues
         * Add angleVelocities to the current object
         *
         * \return a Builder
         */
        CLICK_EXPORT virtual AddControlEventBuilder *withAngleVelocities(std::vector<double> angles) = 0;
        /**
         * @deprecated use withValues
         * Add torques to the current object
         * Can only be used when all Robot joints have the same input signal type
         *
         * \return a Builder
         */
        CLICK_EXPORT virtual AddControlEventBuilder *withTorques(std::vector<double> torques) = 0;
        /**
         * Build the message
         * Can only be used when all Robot joints have the same input signal type
         *
         * \return a ControlMessage
         */
        CLICK_EXPORT virtual std::unique_ptr<ControlMessage> build() = 0;
        CLICK_EXPORT virtual ~AddControlBuilder();
    };

    class AddControlEventBuilder
    {
    public:
        /**
         * Add a control event to current object
         *
         * \return a Builder
         */
        CLICK_EXPORT virtual AddControlEventBuilder *withControlEvent(std::string name, bool activated) = 0;
        /**
         * Add an additional object to Message
         *
         * \return a Builder
         */
        CLICK_EXPORT virtual AddControlBuilder *object(std::string name) = 0;
        /**
         * Build the message
         *
         * \return a ControlMessage
         */
        CLICK_EXPORT virtual std::unique_ptr<ControlMessage> build() = 0;
        CLICK_EXPORT virtual ~AddControlEventBuilder();
    };

    class ControlMessageBuilderImpl : ControlMessageBuilder, AddControlBuilder, AddControlEventBuilder
    {
    public:
        CLICK_EXPORT virtual AddControlBuilder *object(std::string name);
        CLICK_EXPORT virtual AddControlEventBuilder *withAngles(std::vector<double> angles);
        CLICK_EXPORT virtual AddControlEventBuilder *withValues(std::vector<double> values);
        CLICK_EXPORT virtual AddControlEventBuilder *withAngleVelocities(std::vector<double> angles);
        CLICK_EXPORT virtual AddControlEventBuilder *withTorques(std::vector<double> torques);
        CLICK_EXPORT virtual AddControlEventBuilder *withControlEvent(std::string name, bool activated);
        CLICK_EXPORT virtual std::unique_ptr<ControlMessage> build();
        CLICK_EXPORT virtual ~ControlMessageBuilderImpl();
        /**
         * Create a builder that creates a Message
         *
         * \return a Builder
         */
        CLICK_EXPORT static std::unique_ptr<ControlMessageBuilder> builder();

    private:
        ControlMessageBuilderImpl(std::unique_ptr<protobuf::ControlMessage> control_m);
        std::unique_ptr<protobuf::ControlMessage> message;
        protobuf::ControlMessage_Object *currObject;
    };
}
