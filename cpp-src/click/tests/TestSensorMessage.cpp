#include <catch2/catch.hpp>
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
        vector<double> angleVelocities = double_vector_from({2, 3, 4, 5, 6});
        vector<double> torques = double_vector_from({3, 4, 5, 6, 7});
        vector<double> values = double_vector_from({4, 5, 6, 7, 8});

        WHEN("adding a robot with angle controls")
        {
            unique_ptr<SensorMessage> SensorMessage = SensorMessageBuilderImpl::builder()
                ->object("robot1")
                    ->withAngles(angles)
                    ->withAngleVelocities(angleVelocities)
                    ->withTorques(torques)
                ->build();

            THEN("it should have values")
            {
                REQUIRE(SensorMessage->messageType() == MessageType::SensorMessageType);
                REQUIRE(SensorMessage->angles("robot1").size() == 5);
            }
        }
        WHEN("adding a robot with external sensors")
        {
            unique_ptr<SensorMessage> SensorMessage = SensorMessageBuilderImpl::builder()
                ->object("robot1")
                    ->withExternalSensor("external_1")
                        ->withForce({4, 4.1, 4.2})
                        ->withAngularAcceleration({5, 5.1, 5.2})
                ->build();

            THEN("it should have values")
            {
                REQUIRE(SensorMessage->messageType() == MessageType::SensorMessageType);
                REQUIRE(SensorMessage->sensor("robot1", "external_1")[0].force == Vec3{4, 4.1, 4.2});
                REQUIRE(SensorMessage->sensor("robot1", "external_1")[1].angularAcceleration == Vec3{5, 5.1, 5.2});
            }
        }
        WHEN("adding a box with position and rpy")
        {
            unique_ptr<SensorMessage> SensorMessage = SensorMessageBuilderImpl::builder()
                ->object("box")
                    ->withPosition({1, 2, 3})
                    ->withRPY({4, 5, 6})
                ->build();

            THEN("it should have values")
            {
                REQUIRE(SensorMessage->objectPosition("box") == Vec3{1, 2, 3});
                REQUIRE(SensorMessage->objectRPY("box") == Vec3{4, 5, 6});
            }
        }
        WHEN("adding three robots with different controls")
        {
            unique_ptr<SensorMessage> SensorMessage = SensorMessageBuilderImpl::builder()
                ->object("robot1")
                    ->withAngles(angles)
                ->object("robot2")
                    ->withAngleVelocities(angleVelocities)
                ->object("robot3")
                    ->withTorques(torques)
                ->object("box")
                    ->withPosition({1.0, 2.0, 3.0})
                ->build();

            THEN("it should contain the control values")
            {
                REQUIRE(SensorMessage->messageType() == MessageType::SensorMessageType);
                REQUIRE_THAT(SensorMessage->angles("robot1"), Equals(angles));
                REQUIRE_THAT(SensorMessage->angleVelocities("robot2"), Equals(angleVelocities));
                REQUIRE_THAT(SensorMessage->torques("robot3"), Equals(torques));

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
                    "}\n";

                REQUIRE_THAT(SensorMessage->debugString(), Equals(control_facit));
            }

            THEN("it should serialize to string")
            {
                MessageSerializer serializer;
                REQUIRE(serializer.serializeToString(*SensorMessage).length() > 10);
            }

            THEN("it should be deserialized from string")
            {
                MessageSerializer serializer;
                string bytes = serializer.serializeToString(*SensorMessage);

                unique_ptr<Message> message = serializer.fromBytes(bytes);
                REQUIRE(message->messageType() == MessageType::SensorMessageType);
                REQUIRE_THAT(message->debugString(), Equals(SensorMessage->debugString()));
            }
        }
    }
}
