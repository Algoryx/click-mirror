#include <fstream>
#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_all.hpp>
#include <click/MessageSerializer.h>
#include <click/SensorMessage.h>

using namespace std;
using namespace algoryx::click;
// https://github.com/catchorg/Catch2/blob/devel/docs/matchers.md#top
using Catch::Matchers::Contains;
using Catch::Matchers::Equals;


SCENARIO("sensormessage serialization from file", "[clicklib]" ) {

    GIVEN("A serialized sensormessage") {

        string sensorbinary = "../../../testdata/sensormessage.bin";
        fstream input(sensorbinary, ios::in | ios::binary);
        REQUIRE(input);

        WHEN("creating sensormessage from bytes generated by python") {
            MessageSerializer serializer;
            unique_ptr<Message> message = serializer.sensorMessageFromIStream(&input);
            REQUIRE(message->messageType() == SensorMessageType);            
            unique_ptr<SensorMessage> sensorMessage = toSensorMessage(move(message));

            THEN("it should contain robot1 sensor values") {
                REQUIRE_THAT(sensorMessage->debugString(), Equals("SensorMessageType"));
            }

            THEN("it should contain robot1 sensor values") {

                std::vector<double> angles = sensorMessage->angles("robot1");
                std::vector<double> angleVelocities = sensorMessage->angleVelocities("robot1");
                std::vector<double> torques = sensorMessage->torques("robot1");
                REQUIRE(angles == std::vector<double>({1.0}));
                REQUIRE(angleVelocities == std::vector<double>({2.0}));
                REQUIRE(torques == std::vector<double>({3.0}));
                REQUIRE_THAT(sensorMessage->debugString(), Catch::Matchers::StartsWith("messageType: SensorMessageType"));
            }

            THEN("it should contain box position") {

                vector<double> rpy = sensorMessage->objectRPY("box", 0);
//                vector<double> pos = sensorMessage->objectPosition("box", 0);
                vector<double> expected = {4.0, 5.0, 6.0};
                REQUIRE(rpy == expected);
            }
        }
    }
}
