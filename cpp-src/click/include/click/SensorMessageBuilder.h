#pragma once
#include <string>
#include <memory>
#include <click/DllExport.h>
#include <click/SensorMessage.h>

namespace click
{
    namespace protobuf
    {
        class SensorMessage_Object;
    }

    class AddSensorValuesBuilder;

    class SensorMessageBuilder
    {
    public:
        /**
         * Add an additional object to Message
         *
         * \return a Builder
         */
        CLICK_EXPORT virtual AddSensorValuesBuilder *object(std::string name) = 0;
        CLICK_EXPORT virtual ~SensorMessageBuilder();
    };

    class AddSensorValuesBuilder
    {
    public:
        CLICK_EXPORT virtual AddSensorValuesBuilder *withAngles(std::vector<double> angles) = 0;
        CLICK_EXPORT virtual AddSensorValuesBuilder *withAngleVelocities(std::vector<double> angles) = 0;
        CLICK_EXPORT virtual AddSensorValuesBuilder *withTorques(std::vector<double> torques) = 0;
        CLICK_EXPORT virtual AddSensorValuesBuilder *object(std::string name) = 0;
        CLICK_EXPORT virtual std::unique_ptr<SensorMessage> build() = 0;
        CLICK_EXPORT virtual ~AddSensorValuesBuilder();
    };

    class SensorMessageBuilderImpl : SensorMessageBuilder, AddSensorValuesBuilder
    {
    public:
        CLICK_EXPORT virtual AddSensorValuesBuilder *object(std::string name);
        CLICK_EXPORT virtual AddSensorValuesBuilder *withAngles(std::vector<double> angles);
        CLICK_EXPORT virtual AddSensorValuesBuilder *withAngleVelocities(std::vector<double> angles);
        CLICK_EXPORT virtual AddSensorValuesBuilder *withTorques(std::vector<double> torques);
        CLICK_EXPORT virtual std::unique_ptr<SensorMessage> build();
        CLICK_EXPORT virtual ~SensorMessageBuilderImpl();
        /**
         * Create a builder that creates a Message
         *
         * \return a Builder
         */
        CLICK_EXPORT static std::unique_ptr<SensorMessageBuilder> builder();

    private:
        SensorMessageBuilderImpl(std::unique_ptr<protobuf::SensorMessage> control_m);
        std::unique_ptr<protobuf::SensorMessage> message;
        protobuf::SensorMessage_Object *currObject;
    };
}
