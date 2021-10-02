#include <catch2/catch.hpp>
#include <click/Message.h>
#include <Messaging.pb.h>

using namespace std;
using namespace click;

inline void assertEqual(int lhs, int rhs)
{
    REQUIRE(lhs == rhs);
}

SCENARIO("message declarations")
{
    GIVEN("A MessageType and a protobuf::MessageType")
    {
        WHEN("Comparing their values")
        {
            THEN("HandshakeInitMessageType")
            {
                assertEqual(HandshakeInitMessageType, protobuf::HandshakeInitMessageType);
            }
            THEN("HandshakeMessageType")
            {
                assertEqual(HandshakeMessageType, protobuf::HandshakeMessageType);
            }
            THEN("ControlMessageType")
            {
                assertEqual(ControlMessageType, protobuf::ControlMessageType);
            }
            THEN("SensorMessageType")
            {
                assertEqual(SensorMessageType, protobuf::SensorMessageType);
            }
            THEN("ResetMessageType")
            {
                assertEqual(ResetMessageType, protobuf::ResetMessageType);
            }
            THEN("ErrorMessageType")
            {
                assertEqual(ErrorMessageType, protobuf::ErrorMessageType);
            }
        }
    }

    GIVEN("A ValueType and a protobuf::ValueType")
    {
        WHEN("Comparing their values")
        {
            THEN("Angle")
            {
                assertEqual(Angle, protobuf::Angle);
            }
            THEN("AngleVelocity")
            {
                assertEqual(AngleVelocity, protobuf::AngleVelocity);
            }
            THEN("Torque")
            {
                assertEqual(Torque, protobuf::Torque);
            }
            THEN("Position")
            {
                assertEqual(Position, protobuf::Position);
            }
            THEN("RPY")
            {
                assertEqual(RPY, protobuf::RPY);
            }
            THEN("Activated")
            {
                assertEqual(Activated, protobuf::Activated);
            }
            THEN("Acceleration")
            {
                assertEqual(Acceleration, protobuf::Acceleration);
            }
            THEN("Force")
            {
                assertEqual(Force, protobuf::Force);
            }
            THEN("Angle")
            {
                assertEqual(DirectionalTorque, protobuf::DirectionalTorque);
            }
            THEN("AngularAcceleration")
            {
                assertEqual(AngularAcceleration, protobuf::AngularAcceleration);
            }
        }
    }
}
