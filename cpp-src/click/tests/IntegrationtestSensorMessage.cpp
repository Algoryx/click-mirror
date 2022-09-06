#include <fstream>
#include <catch2/catch.hpp>
#include <click/MessageSerializer.h>
#include <click/SensorMessage.h>
#include "TestPaths.h"
using namespace std;
using namespace click;
using Catch::Matchers::Contains;
using Catch::Matchers::Equals;

SCENARIO("sensormessage serialization from file", "[click]")
{
    GIVEN("A serialized sensormessage")
    {
        // This file is created by test_write_sensor_message_to_file in test_messaging.py
        fstream input(TESTDATA_DIR + "sensormessage.bin", ios::in | ios::binary);
        REQUIRE(input);

        WHEN("creating sensormessage from bytes generated by python")
        {
            MessageSerializer serializer;
            unique_ptr<Message> message = serializer.sensorMessageFromIStream(&input);
            REQUIRE(message->messageType() == MessageType::SensorMessageType);
            unique_ptr<SensorMessage> sensorMessage = toSensorMessage(move(message));

            THEN("it should have debugstring")
            {
                REQUIRE_THAT(sensorMessage->debugString(), Contains("messageType: SensorMessageType"));
            }

            THEN("it should contain simulated time")
            {
                REQUIRE(sensorMessage->simulatedTime() == 0.0);
            }

            THEN("robot1 should have angles")
            {
                REQUIRE(sensorMessage->angles("robot1") == std::vector<double>{1.0, 1.1});
            }

            AND_THEN("robot1 should have angleVelocities")
            {
                REQUIRE(sensorMessage->angleVelocities("robot1") == std::vector<double>{2.0, 2.1});
            }

            AND_THEN("robot1 should have torques")
            {
                REQUIRE(sensorMessage->torques("robot1") == std::vector<double>{3.0, 3.1});
            }

            AND_THEN("robot1 should have list of external sensors")
            {
                vector<Sensor> sensors = sensorMessage->sensor("robot1", "external_1");
                REQUIRE(sensors.at(0).force == Vec3({4.0,4.1,4.2}));
                REQUIRE(sensors.at(1).angularAcceleration == Vec3({5.0,5.1,5.2}));
            }
            AND_THEN("robot1 should have external sensors individually accessible")
            {
                Vec3 v = sensorMessage->sensorVec3("robot1", "external_1", 0);
                REQUIRE(v == Vec3({4.0,4.1,4.2}));
                v = sensorMessage->sensorVec3("robot1", "external_1", 1);
                REQUIRE(v == Vec3({5.0,5.1,5.2}));
            }
            AND_THEN("robot2 should not be found")
            {
                REQUIRE_THROWS_WITH(sensorMessage->torques("robot2"), "CHECK failed: it != end(): key not found: robot2" );
            }
            AND_THEN("box should have roll pitch yaw")
            {
                REQUIRE(sensorMessage->objectRPY("box") == Vec3{4.0, 5.0, 6.0});
            }
            AND_THEN("box should have position")
            {
                REQUIRE(sensorMessage->objectPosition("box") == Vec3{1.0, 2.0, 3.0});
            }

            THEN("it should roundtrip serialize to/from bytes")
            {
                MessageSerializer serializer;
                string bytes = serializer.serializeToString(*sensorMessage);
                unique_ptr<Message> message = serializer.fromBytes(bytes);
                REQUIRE_THAT(message->debugString(), Equals(sensorMessage->debugString()));
            }
        }
    }
}
