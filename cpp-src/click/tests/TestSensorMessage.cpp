#include <catch2/catch_all.hpp>
#include <click/SensorMessage.h>
#include <click/SensorMessageBuilder.h>
#include <click/MessageSerializer.h>
#include <Messaging.pb.h>

using namespace std;
using namespace click;
using Catch::Matchers::Equals;

inline vector<double> double_vector_from(initializer_list<double> doubles)
{
    return vector<double>(doubles);
}

SCENARIO("Sensormessage serialization", "[click]")
{
    GIVEN("A Sensormessage")
    {
        vector<double> angles = double_vector_from({1, 2, 3, 4, 5});
        vector<double> angularVelocities = double_vector_from({2, 3, 4, 5, 6});
        vector<double> torques = double_vector_from({3, 4, 5, 6, 7});
        vector<double> values = double_vector_from({4, 5, 6, 7, 8});

        WHEN("adding a robot with angle controls")
        {
            unique_ptr<SensorMessage> ensor_message = SensorMessageBuilderImpl::builder()
                ->withSimulatedTime(1.2)
                ->object("robot1")
                    ->withAngles(angles)
                    ->withAngularVelocities(angularVelocities)
                    ->withTorques(torques)
                ->build();

            THEN("it should have values")
            {
                REQUIRE(ensor_message->messageType() == MessageType::SensorMessageType);
                REQUIRE(ensor_message->simulatedTime() == 1.2);
                REQUIRE(ensor_message->angles("robot1").size() == 5);
            }
        }
        WHEN("adding a robot with external sensors")
        {
            unique_ptr<SensorMessage> sensor_message = SensorMessageBuilderImpl::builder()
                ->object("robot1")
                    ->withSensor("external_1")
                        ->withAcceleration({3, 3.1, 3.2})
                        ->withActivated(true)
                        ->withAngle(1.1)
                        ->withAngularVelocity(2.2)
                        ->withAngularAcceleration({5, 5.1, 5.2})
                        ->withDirectionalTorque({6, 6.1, 6.2})
                        ->withForce({4, 4.1, 4.2})
                        ->withPosition_({7, 7.1, 7.2})
                        ->withRPY_({8, 8.1, 8.2})
                        ->withTorque(9)
                ->build();

            THEN("it should have values")
            {
                REQUIRE(sensor_message->messageType() == MessageType::SensorMessageType);
                auto sensor_vals = sensor_message->sensor("robot1", "external_1");
                REQUIRE(sensor_vals[0].type == click::ValueType::Acceleration);
                REQUIRE(sensor_vals[0].value.acceleration == Vec3{3, 3.1, 3.2});
                REQUIRE(sensor_vals[1].type == click::ValueType::Activated);
                REQUIRE(sensor_vals[1].value.activated == true);
                REQUIRE(sensor_vals[2].type == click::ValueType::Angle);
                REQUIRE(sensor_vals[2].value.angle == 1.1);
                REQUIRE(sensor_vals[3].type == click::ValueType::AngularVelocity);
                REQUIRE(sensor_vals[3].value.angularVelocity == 2.2);
                REQUIRE(sensor_vals[4].type == click::ValueType::AngularAcceleration);
                REQUIRE(sensor_vals[4].value.angularAcceleration == Vec3{5, 5.1, 5.2});
                REQUIRE(sensor_vals[5].type == click::ValueType::DirectionalTorque);
                REQUIRE(sensor_vals[5].value.directionalTorque == Vec3{6, 6.1, 6.2});
                REQUIRE(sensor_vals[6].type == click::ValueType::Force);
                REQUIRE(sensor_vals[6].value.force == Vec3{4, 4.1, 4.2});
                REQUIRE(sensor_vals[7].type == click::ValueType::Position);
                REQUIRE(sensor_vals[7].value.position == Vec3{7, 7.1, 7.2});
                REQUIRE(sensor_vals[8].type == click::ValueType::RPY);
                REQUIRE(sensor_vals[8].value.rpy == Vec3{8, 8.1, 8.2});
            }
        }
        WHEN("adding a box with position and rpy")
        {
            unique_ptr<SensorMessage> sensor_message = SensorMessageBuilderImpl::builder()
                ->object("box")
                    ->withPosition({1, 2, 3})
                    ->withRPY({4, 5, 6})
                ->build();

            THEN("it should have values")
            {
                REQUIRE(sensor_message->objectPosition("box") == Vec3{1, 2, 3});
                REQUIRE(sensor_message->objectRPY("box") == Vec3{4, 5, 6});
            }
        }
        WHEN("adding three robots with different controls")
        {
            unique_ptr<SensorMessage> sensor_message = SensorMessageBuilderImpl::builder()
                ->withSimulatedTime(1.3)
                ->object("robot1")
                    ->withAngles(angles)
                ->object("robot2")
                    ->withAngularVelocities(angularVelocities)
                ->object("robot3")
                    ->withTorques(torques)
                ->object("box")
                    ->withPosition({1.0, 2.0, 3.0})
                ->build();

            THEN("it should contain the control values")
            {
                REQUIRE(sensor_message->messageType() == MessageType::SensorMessageType);
                REQUIRE_THAT(sensor_message->angles("robot1"), Equals(angles));
                REQUIRE_THAT(sensor_message->angularVelocities("robot2"), Equals(angularVelocities));
                REQUIRE_THAT(sensor_message->torques("robot3"), Equals(torques));

                string control_facit =
                    "messageType: SensorMessageType\n"
                    "objects {\n"
                    "  key: \"box\"\n"
                    "  value {\n"
                    "    objectSensors {\n"
                    "      position {\n"
                    "        arr: 1\n"
                    "        arr: 2\n"
                    "        arr: 3\n"
                    "      }\n"
                    "    }\n"
                    "  }\n"
                    "}\n"
                    "objects {\n"
                    "  key: \"robot1\"\n"
                    "  value {\n"
                    "    angleSensors: 1\n"
                    "    angleSensors: 2\n"
                    "    angleSensors: 3\n"
                    "    angleSensors: 4\n"
                    "    angleSensors: 5\n"
                    "  }\n"
                    "}\n"
                    "objects {\n"
                    "  key: \"robot2\"\n"
                    "  value {\n"
                    "    angleVelocitySensors: 2\n"
                    "    angleVelocitySensors: 3\n"
                    "    angleVelocitySensors: 4\n"
                    "    angleVelocitySensors: 5\n"
                    "    angleVelocitySensors: 6\n"
                    "  }\n"
                    "}\n"
                    "objects {\n"
                    "  key: \"robot3\"\n"
                    "  value {\n"
                    "    torqueSensors: 3\n"
                    "    torqueSensors: 4\n"
                    "    torqueSensors: 5\n"
                    "    torqueSensors: 6\n"
                    "    torqueSensors: 7\n"
                    "  }\n"
                    "}\n"
                    "simVars {\n"
                    "  simulatedTime: 1.3\n"
                    "}\n";

                REQUIRE_THAT(sensor_message->debugString(), Equals(control_facit));
            }

            THEN("it should serialize to string")
            {
                MessageSerializer serializer;
                REQUIRE(serializer.serializeToString(*sensor_message).length() > 10);
            }

            THEN("it should be deserialized from string")
            {
                MessageSerializer serializer;
                string bytes = serializer.serializeToString(*sensor_message);

                unique_ptr<Message> message = serializer.fromBytes(bytes);
                REQUIRE(message->messageType() == MessageType::SensorMessageType);
                REQUIRE_THAT(message->debugString(), Equals(sensor_message->debugString()));
            }
        }
    }
}
