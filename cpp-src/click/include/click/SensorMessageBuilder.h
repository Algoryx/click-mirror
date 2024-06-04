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

    class CLICK_EXPORT SensorMessageBuilder
    {
    public:
        /**
         * Add an additional object to Message
         *
         * \return a Builder
         */
        virtual AddSensorValuesBuilder *object(const std::string& name) = 0;
        virtual AddSensorValuesBuilder *withSimulatedTime(double time) = 0;
        virtual ~SensorMessageBuilder();
    };

    class CLICK_EXPORT AddSensorValuesBuilder
    {
    public:
        virtual AddSensorValuesBuilder* object(const std::string& name) = 0;
        virtual AddSensorValuesBuilder* withAngles(const std::vector<double>& angles) = 0;
        virtual AddSensorValuesBuilder* withAngularVelocities(const std::vector<double>& angles) = 0;
        virtual AddSensorValuesBuilder* withTorques(const std::vector<double>& torques) = 0;
        virtual AddSensorValuesBuilder* withPosition(const Vec3& vec3) = 0;
        virtual AddSensorValuesBuilder* withRPY(const Vec3& vec3) = 0;
        virtual AddSensorBuilder* withSensor(const std::string& name) = 0;
        virtual std::unique_ptr<SensorMessage> build() = 0;
        virtual ~AddSensorValuesBuilder();
    };

    class CLICK_EXPORT AddSensorBuilder
    {
        public:
        virtual AddSensorBuilder* withAngle(double angle) = 0;
        virtual AddSensorBuilder* withAngularVelocity1d(double angular_velocity) = 0;
        virtual AddSensorBuilder* withTorque1d(double torque) = 0;
        virtual AddSensorBuilder* withPosition_(const Vec3& position) = 0;
        virtual AddSensorBuilder* withRPY_(const Vec3& rpy) = 0;
        virtual AddSensorBuilder* withActivated(bool activated) = 0;
        virtual AddSensorBuilder* withAcceleration3d(const Vec3& acceleration) = 0;
        virtual AddSensorBuilder* withForce3d(const Vec3& vec3) = 0;
        virtual AddSensorBuilder* withTorque3d(const Vec3& vec3) = 0;
        virtual AddSensorBuilder* withAngularAcceleration3d(const Vec3& vec3) = 0;
        virtual AddSensorBuilder* withVelocity3d(const Vec3& vec3) = 0;
        virtual AddSensorBuilder* withAngularVelocity3d(const Vec3& vec3) = 0;
        virtual std::unique_ptr<SensorMessage> build() = 0;
        virtual AddSensorValuesBuilder* object(const std::string& name) = 0;
    };

    class CLICK_EXPORT SensorMessageBuilderImpl : SensorMessageBuilder, AddSensorValuesBuilder, AddSensorBuilder
    {
    public:
        virtual AddSensorValuesBuilder* object(const std::string& name);
        virtual AddSensorValuesBuilder *withSimulatedTime(double time);
        virtual AddSensorValuesBuilder* withAngles(const std::vector<double>& angles);
        virtual AddSensorValuesBuilder* withAngularVelocities(const std::vector<double>& angles);
        virtual AddSensorValuesBuilder* withTorques(const std::vector<double>& torques);
        virtual AddSensorValuesBuilder* withPosition(const Vec3& vec3);
        virtual AddSensorValuesBuilder* withRPY(const Vec3& vec3);

        virtual AddSensorBuilder* withSensor(const std::string& name);
        virtual AddSensorBuilder* withAngle(double angle);
        virtual AddSensorBuilder* withAngularVelocity1d(double angular_velocity);
        virtual AddSensorBuilder* withTorque1d(double torque);
        virtual AddSensorBuilder* withPosition_(const Vec3& position);
        virtual AddSensorBuilder* withRPY_(const Vec3& rpy);
        virtual AddSensorBuilder* withActivated(bool activated);
        virtual AddSensorBuilder* withAcceleration3d(const Vec3& acceleration);
        virtual AddSensorBuilder* withForce3d(const Vec3& vec3);
        virtual AddSensorBuilder* withTorque3d(const Vec3& vec3);
        virtual AddSensorBuilder* withAngularAcceleration3d(const Vec3& vec3);
        virtual AddSensorBuilder* withVelocity3d(const Vec3& vec3);
        virtual AddSensorBuilder* withAngularVelocity3d(const Vec3& vec3);
        virtual std::unique_ptr<SensorMessage> build();
        virtual ~SensorMessageBuilderImpl();
        /**
         * Create a builder that creates a Message
         *
         * \return a Builder
         */
        static std::unique_ptr<SensorMessageBuilder> builder();

    private:
        SensorMessageBuilderImpl(std::unique_ptr<protobuf::SensorMessage> control_m);
        std::unique_ptr<protobuf::SensorMessage> m_message;
        protobuf::SensorMessage_Object *m_curr_object;
        protobuf::SensorMessage_Sensors *m_curr_sensor;
    };
}
