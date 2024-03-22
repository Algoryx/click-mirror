#include <catch2/catch_all.hpp>
#include <click/Message.h>
#include <Messaging.pb.h>

using namespace std;
using namespace click;

inline void assertEqual(MessageType lhs, protobuf::MessageType rhs)
{
    REQUIRE(static_cast<int>(lhs) == rhs);
}

inline void assertEqual(ValueType lhs, protobuf::ValueType rhs)
{
    REQUIRE(static_cast<int>(lhs) == rhs);
}

SCENARIO("message declarations")
{
    GIVEN("A MessageType and a protobuf::MessageType")
    {
        WHEN("Comparing their values")
        {
            THEN("HandshakeInitMessageType")
            {
                assertEqual(MessageType::HandshakeInitMessageType, protobuf::HandshakeInitMessageType);
            }
            THEN("HandshakeMessageType")
            {
                assertEqual(MessageType::HandshakeMessageType, protobuf::HandshakeMessageType);
            }
            THEN("ControlMessageType")
            {
                assertEqual(MessageType::ControlMessageType, protobuf::ControlMessageType);
            }
            THEN("SensorMessageType")
            {
                assertEqual(MessageType::SensorMessageType, protobuf::SensorMessageType);
            }
            THEN("ResetMessageType")
            {
                assertEqual(MessageType::ResetMessageType, protobuf::ResetMessageType);
            }
            THEN("ErrorMessageType")
            {
                assertEqual(MessageType::ErrorMessageType, protobuf::ErrorMessageType);
            }
        }
    }

    GIVEN("A ValueType and a protobuf::ValueType")
    {
        WHEN("Comparing their values")
        {
            THEN("Angle")
            {
                assertEqual(ValueType::Angle, protobuf::Angle);
            }
            THEN("AngleVelocity")
            {
                assertEqual(ValueType::AngularVelocity, protobuf::AngleVelocity);
            }
            THEN("Torque")
            {
                assertEqual(ValueType::Torque, protobuf::Torque);
            }
            THEN("Position")
            {
                assertEqual(ValueType::Position, protobuf::Position);
            }
            THEN("RPY")
            {
                assertEqual(ValueType::RPY, protobuf::RPY);
            }
            THEN("Activated")
            {
                assertEqual(ValueType::Activated, protobuf::Activated);
            }
            THEN("Acceleration")
            {
                assertEqual(ValueType::Acceleration, protobuf::Acceleration);
            }
            THEN("Force")
            {
                assertEqual(ValueType::Force, protobuf::Force);
            }
            THEN("DirectionalTorque")
            {
                assertEqual(ValueType::DirectionalTorque, protobuf::DirectionalTorque);
            }
            THEN("AngularAcceleration")
            {
                assertEqual(ValueType::AngularAcceleration, protobuf::AngularAcceleration);
            }
            THEN("Multiple")
            {
                assertEqual(ValueType::Multiple, protobuf::Multiple);
            }
        }
    }
}
