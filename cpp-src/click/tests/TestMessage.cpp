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
            THEN("AngleVelocity1D")
            {
                assertEqual(ValueType::AngularVelocity1D, protobuf::AngularVelocity1D);
            }
            THEN("Torque1D")
            {
                assertEqual(ValueType::Torque1D, protobuf::Torque1D);
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
            THEN("Acceleration3D")
            {
                assertEqual(ValueType::Acceleration3D, protobuf::Acceleration3D);
            }
            THEN("Force1D")
            {
                assertEqual(ValueType::Force1D, protobuf::Force1D);
            }
            THEN("Force3D")
            {
                assertEqual(ValueType::Force3D, protobuf::Force3D);
            }
            THEN("Torque3D")
            {
                assertEqual(ValueType::Torque3D, protobuf::Torque3D);
            }
            THEN("AngularAcceleration3D")
            {
                assertEqual(ValueType::AngularAcceleration3D, protobuf::AngularAcceleration3D);
            }
            THEN("Velocity3D")
            {
                assertEqual(ValueType::Velocity3D, protobuf::Velocity3D);
            }
            THEN("AngularVelocity3D")
            {
                assertEqual(ValueType::AngularVelocity3D, protobuf::AngularVelocity3D);
            }
            THEN("Multiple")
            {
                assertEqual(ValueType::Multiple, protobuf::Multiple);
            }
        }
    }
}
