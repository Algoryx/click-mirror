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
        class SensorMessage_Sensors;
    }

    class AddSensorValuesBuilder;
    class AddExternalSensorBuilder;

    class SensorMessageBuilder
    {
    public:
        /**
         * Add an additional object to Message
         *
         * \return a Builder
         */
        CLICK_EXPORT virtual AddSensorValuesBuilder *object(const std::string& name) = 0;
        CLICK_EXPORT virtual ~SensorMessageBuilder();
    };

    class AddSensorValuesBuilder
    {
    public:
        CLICK_EXPORT virtual AddSensorValuesBuilder* object(const std::string& name) = 0;
        CLICK_EXPORT virtual AddSensorValuesBuilder* withAngles(const std::vector<double>& angles) = 0;
        CLICK_EXPORT virtual AddSensorValuesBuilder* withAngleVelocities(const std::vector<double>& angles) = 0;
        CLICK_EXPORT virtual AddSensorValuesBuilder* withTorques(const std::vector<double>& torques) = 0;
        CLICK_EXPORT virtual AddSensorValuesBuilder* withPosition(const Vec3& vec3) = 0;
        CLICK_EXPORT virtual AddSensorValuesBuilder* withRPY(const Vec3& vec3) = 0;
        CLICK_EXPORT virtual AddExternalSensorBuilder* withExternalSensor(const std::string& name) = 0;
        CLICK_EXPORT virtual std::unique_ptr<SensorMessage> build() = 0;
        CLICK_EXPORT virtual ~AddSensorValuesBuilder();
    };

    class AddExternalSensorBuilder
    {
        public:
        CLICK_EXPORT virtual AddExternalSensorBuilder* withForce(const Vec3& vec3) = 0;
        CLICK_EXPORT virtual AddExternalSensorBuilder* withAngularAcceleration(const Vec3& vec3) = 0;
        CLICK_EXPORT virtual std::unique_ptr<SensorMessage> build() = 0;
        CLICK_EXPORT virtual AddSensorValuesBuilder* object(const std::string& name) = 0;
    };

    class SensorMessageBuilderImpl : SensorMessageBuilder, AddSensorValuesBuilder, AddExternalSensorBuilder
    {
    public:
        CLICK_EXPORT virtual AddSensorValuesBuilder* object(const std::string& name);
        CLICK_EXPORT virtual AddSensorValuesBuilder* withAngles(const std::vector<double>& angles);
        CLICK_EXPORT virtual AddSensorValuesBuilder* withAngleVelocities(const std::vector<double>& angles);
        CLICK_EXPORT virtual AddSensorValuesBuilder* withTorques(const std::vector<double>& torques);
        CLICK_EXPORT virtual AddSensorValuesBuilder* withPosition(const Vec3& vec3);
        CLICK_EXPORT virtual AddSensorValuesBuilder* withRPY(const Vec3& vec3);
        CLICK_EXPORT virtual AddExternalSensorBuilder* withExternalSensor(const std::string& name);
        CLICK_EXPORT virtual AddExternalSensorBuilder* withForce(const Vec3& vec3);
        CLICK_EXPORT virtual AddExternalSensorBuilder* withAngularAcceleration(const Vec3& vec3);
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
        protobuf::SensorMessage_Sensors *curr_sensor;
    };
}
