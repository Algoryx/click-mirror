#pragma once
#include<string>
#include<memory>
#include <click/HandshakeMessage.h>

namespace click {

namespace protobuf {
    class HandshakeMessage_Object;
    class HandshakeMessage_Sensors;
}


class AddRobotBuilder;
class AddHandshakeSensorBuilder;

class HandshakeMessageBuilder {

public:
    CLICK_EXPORT virtual std::unique_ptr<HandshakeMessage> build() = 0;
    CLICK_EXPORT virtual HandshakeMessageBuilder * withSimulationSettings(const click::HandshakeMessage::SimulationSettings & simulation_settings) = 0;
    CLICK_EXPORT virtual HandshakeMessageBuilder * withControlType(const click::ValueType & control_type) = 0;
    CLICK_EXPORT virtual AddRobotBuilder * withRobot(const std::string & name) = 0;
    CLICK_EXPORT virtual ~HandshakeMessageBuilder();
};

class AddRobotBuilder {

public:
    CLICK_EXPORT virtual std::unique_ptr<HandshakeMessage> build() = 0;
    CLICK_EXPORT virtual AddRobotBuilder * withControlsInOrder(const std::vector<std::string> & order) = 0;
    CLICK_EXPORT virtual AddRobotBuilder * withControlTypesInOrder(const std::vector<click::ValueType> & order) = 0;
    CLICK_EXPORT virtual AddRobotBuilder * withJointSensorsInOrder(const std::vector<std::string> & order) = 0;
    CLICK_EXPORT virtual AddRobotBuilder * withJointSensors(const std::vector<click::ValueType> & order) = 0;
    CLICK_EXPORT virtual AddHandshakeSensorBuilder * withSensor(const std::string & name) = 0;
    CLICK_EXPORT virtual AddRobotBuilder * withControlEvent(const std::string & name, const click::ValueType & type) = 0;
    CLICK_EXPORT virtual AddRobotBuilder * withObjectSensors(std::vector<click::ValueType> & order) = 0;
    CLICK_EXPORT virtual AddRobotBuilder * withRobot(const std::string & name) = 0;
    CLICK_EXPORT virtual ~AddRobotBuilder();
};

class AddHandshakeSensorBuilder {

public:
    CLICK_EXPORT virtual std::unique_ptr<HandshakeMessage> build() = 0;
    CLICK_EXPORT virtual AddRobotBuilder * withTypesInOrder(const std::vector<click::ValueType> & order) = 0;
    CLICK_EXPORT virtual ~AddHandshakeSensorBuilder();
};

class HandshakeMessageBuilderImpl : public HandshakeMessageBuilder, AddRobotBuilder, AddHandshakeSensorBuilder {

public:
    CLICK_EXPORT std::unique_ptr<HandshakeMessage> build();
    /**
     * Create a builder that creates a Message
     *
     * \return a Builder
     */
    CLICK_EXPORT static std::unique_ptr<HandshakeMessageBuilder> builder();
    // HandshakeMessageBuilder
    CLICK_EXPORT virtual HandshakeMessageBuilder * withSimulationSettings(const click::HandshakeMessage::SimulationSettings & simulation_settings);
    CLICK_EXPORT virtual HandshakeMessageBuilder * withControlType(const click::ValueType & control_type);
    CLICK_EXPORT virtual AddRobotBuilder * withRobot(const std::string & name);
    // AddRobotBuilder
    CLICK_EXPORT virtual AddRobotBuilder * withControlsInOrder(const std::vector<std::string> & order);
    CLICK_EXPORT virtual AddRobotBuilder * withControlTypesInOrder(const std::vector<click::ValueType> & order);
    CLICK_EXPORT virtual AddRobotBuilder * withJointSensorsInOrder(const std::vector<std::string> & order);
    CLICK_EXPORT virtual AddRobotBuilder * withJointSensors(const std::vector<click::ValueType> & order);
    CLICK_EXPORT virtual AddHandshakeSensorBuilder * withSensor(const std::string & name);
    CLICK_EXPORT virtual AddRobotBuilder * withControlEvent(const std::string & name, const click::ValueType & type);
    CLICK_EXPORT virtual AddRobotBuilder * withObjectSensors(std::vector<click::ValueType> & order);
    // AddHandshakeSensorBuilder
    CLICK_EXPORT virtual AddRobotBuilder * withTypesInOrder(const std::vector<click::ValueType> & order);
    CLICK_EXPORT virtual ~HandshakeMessageBuilderImpl();

private:
    HandshakeMessageBuilderImpl(std::unique_ptr<protobuf::HandshakeMessage> control_m);
    std::unique_ptr<protobuf::HandshakeMessage> m_message;
    protobuf::HandshakeMessage_Object *m_curr_object;
    protobuf::HandshakeMessage_Sensors *m_curr_sensor;
};
}
