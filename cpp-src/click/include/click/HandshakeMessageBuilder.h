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

class CLICK_EXPORT HandshakeMessageBuilder {

public:
    virtual std::unique_ptr<HandshakeMessage> build() = 0;
    virtual HandshakeMessageBuilder* withSimulationSettings(const click::HandshakeMessage::SimulationSettings&  simulation_settings) = 0;
    virtual HandshakeMessageBuilder* withControlType(const click::ValueType&  control_type) = 0;
    virtual AddRobotBuilder* withRobot(const std::string&  name) = 0;
    virtual ~HandshakeMessageBuilder();
};

class CLICK_EXPORT AddRobotBuilder {

public:
    virtual std::unique_ptr<HandshakeMessage> build() = 0;
    virtual AddRobotBuilder* withControlsInOrder(const std::vector<std::string>&  order) = 0;
    virtual AddRobotBuilder* withControlTypesInOrder(const std::vector<click::ValueType>&  order) = 0;
    virtual AddRobotBuilder* withJointSensorsInOrder(const std::vector<std::string>&  order) = 0;
    virtual AddRobotBuilder* withJointSensors(const std::vector<click::ValueType>&  order) = 0;
    virtual AddHandshakeSensorBuilder* withSensor(const std::string&  name) = 0;
    virtual AddRobotBuilder* withControlEvent(const std::string&  name, const click::ValueType&  type) = 0;
    virtual AddRobotBuilder* withObjectSensors(std::vector<click::ValueType>&  order) = 0;
    virtual AddRobotBuilder* withRobot(const std::string&  name) = 0;
    virtual ~AddRobotBuilder();
};

class CLICK_EXPORT AddHandshakeSensorBuilder {

public:
    virtual std::unique_ptr<HandshakeMessage> build() = 0;
    virtual AddRobotBuilder* withTypesInOrder(const std::vector<click::ValueType>&  order) = 0;
    virtual ~AddHandshakeSensorBuilder();
};

class CLICK_EXPORT HandshakeMessageBuilderImpl : public HandshakeMessageBuilder, AddRobotBuilder, AddHandshakeSensorBuilder {

public:
    std::unique_ptr<HandshakeMessage> build();
    /**
     * Create a builder that creates a Message
     *
     * \return a Builder
     */
    static std::unique_ptr<HandshakeMessageBuilder> builder();
    // HandshakeMessageBuilder
    virtual HandshakeMessageBuilder* withSimulationSettings(const click::HandshakeMessage::SimulationSettings&  simulation_settings);
    virtual HandshakeMessageBuilder* withControlType(const click::ValueType&  control_type);
    virtual AddRobotBuilder* withRobot(const std::string&  name);
    // AddRobotBuilder
    virtual AddRobotBuilder* withControlsInOrder(const std::vector<std::string>&  order);
    virtual AddRobotBuilder* withControlTypesInOrder(const std::vector<click::ValueType>&  order);
    virtual AddRobotBuilder* withJointSensorsInOrder(const std::vector<std::string>&  order);
    virtual AddRobotBuilder* withJointSensors(const std::vector<click::ValueType>&  order);
    virtual AddHandshakeSensorBuilder* withSensor(const std::string&  name);
    virtual AddRobotBuilder* withControlEvent(const std::string&  name, const click::ValueType&  type);
    virtual AddRobotBuilder* withObjectSensors(std::vector<click::ValueType>&  order);
    // AddHandshakeSensorBuilder
    virtual AddRobotBuilder* withTypesInOrder(const std::vector<click::ValueType>&  order);
    virtual ~HandshakeMessageBuilderImpl();

private:
    HandshakeMessageBuilderImpl(std::unique_ptr<protobuf::HandshakeMessage> control_m);
    std::unique_ptr<protobuf::HandshakeMessage> m_message;
    protobuf::HandshakeMessage_Object* m_curr_object;
    protobuf::HandshakeMessage_Sensors* m_curr_sensor;
};
}
