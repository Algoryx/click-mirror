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
    class AddSensorBuilder;

    class SensorMessageBuilder
    {
    public:
        /**
         * Add an additional object to Message
         *
         * \return a Builder
         */
        CLICK_EXPORT virtual AddSensorValuesBuilder *object(const std::string& name) = 0;
        CLICK_EXPORT virtual AddSensorValuesBuilder *withSimulatedTime(double time) = 0;
        CLICK_EXPORT virtual ~SensorMessageBuilder();
    };

    class AddSensorValuesBuilder
    {
    public:
        CLICK_EXPORT virtual AddSensorValuesBuilder* object(const std::string& name) = 0;
        CLICK_EXPORT virtual AddSensorValuesBuilder* withAngles(const std::vector<double>& angles) = 0;
        CLICK_EXPORT virtual AddSensorValuesBuilder* withAngularVelocities(const std::vector<double>& angles) = 0;
        CLICK_EXPORT virtual AddSensorValuesBuilder* withTorques(const std::vector<double>& torques) = 0;
        CLICK_EXPORT virtual AddSensorValuesBuilder* withPosition(const Vec3& vec3) = 0;
        CLICK_EXPORT virtual AddSensorValuesBuilder* withRPY(const Vec3& vec3) = 0;
        CLICK_EXPORT virtual AddSensorBuilder* withSensor(const std::string& name) = 0;
        CLICK_EXPORT virtual std::unique_ptr<SensorMessage> build() = 0;
        CLICK_EXPORT virtual ~AddSensorValuesBuilder();
    };

    class AddSensorBuilder
    {
        public:
        CLICK_EXPORT virtual AddSensorBuilder* withAngle(double angle) = 0;
        CLICK_EXPORT virtual AddSensorBuilder* withAngularVelocity(double angular_velocity) = 0;
        CLICK_EXPORT virtual AddSensorBuilder* withTorque(double torque) = 0;
        CLICK_EXPORT virtual AddSensorBuilder* withPosition_(const Vec3& position) = 0;
        CLICK_EXPORT virtual AddSensorBuilder* withRPY_(const Vec3& rpy) = 0;
        CLICK_EXPORT virtual AddSensorBuilder* withActivated(bool activated) = 0;
        CLICK_EXPORT virtual AddSensorBuilder* withAcceleration(const Vec3& acceleration) = 0;
        CLICK_EXPORT virtual AddSensorBuilder* withForce(const Vec3& vec3) = 0;
        CLICK_EXPORT virtual AddSensorBuilder* withDirectionalTorque(const Vec3& vec3) = 0;
        CLICK_EXPORT virtual AddSensorBuilder* withAngularAcceleration(const Vec3& vec3) = 0;
        CLICK_EXPORT virtual std::unique_ptr<SensorMessage> build() = 0;
        CLICK_EXPORT virtual AddSensorValuesBuilder* object(const std::string& name) = 0;
    };

    class SensorMessageBuilderImpl : SensorMessageBuilder, AddSensorValuesBuilder, AddSensorBuilder
    {
    public:
        CLICK_EXPORT virtual AddSensorValuesBuilder* object(const std::string& name);
        CLICK_EXPORT virtual AddSensorValuesBuilder *withSimulatedTime(double time);
        CLICK_EXPORT virtual AddSensorValuesBuilder* withAngles(const std::vector<double>& angles);
        CLICK_EXPORT virtual AddSensorValuesBuilder* withAngularVelocities(const std::vector<double>& angles);
        CLICK_EXPORT virtual AddSensorValuesBuilder* withTorques(const std::vector<double>& torques);
        CLICK_EXPORT virtual AddSensorValuesBuilder* withPosition(const Vec3& vec3);
        CLICK_EXPORT virtual AddSensorValuesBuilder* withRPY(const Vec3& vec3);

        CLICK_EXPORT virtual AddSensorBuilder* withSensor(const std::string& name);
        CLICK_EXPORT virtual AddSensorBuilder* withAngle(double angle);
        CLICK_EXPORT virtual AddSensorBuilder* withAngularVelocity(double angular_velocity);
        CLICK_EXPORT virtual AddSensorBuilder* withTorque(double torque);
        CLICK_EXPORT virtual AddSensorBuilder* withPosition_(const Vec3& position);
        CLICK_EXPORT virtual AddSensorBuilder* withRPY_(const Vec3& rpy);
        CLICK_EXPORT virtual AddSensorBuilder* withActivated(bool activated);
        CLICK_EXPORT virtual AddSensorBuilder* withAcceleration(const Vec3& acceleration);
        CLICK_EXPORT virtual AddSensorBuilder* withForce(const Vec3& vec3);
        CLICK_EXPORT virtual AddSensorBuilder* withDirectionalTorque(const Vec3& vec3);
        CLICK_EXPORT virtual AddSensorBuilder* withAngularAcceleration(const Vec3& vec3);
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
        std::unique_ptr<protobuf::SensorMessage> m_message;
        protobuf::SensorMessage_Object *m_curr_object;
        protobuf::SensorMessage_Sensors *m_curr_sensor;
    };
}
