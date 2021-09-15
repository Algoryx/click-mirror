#include <fstream>
#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_all.hpp>
#include "Messaging.pb.h"

using namespace std;
using namespace iam;
// https://github.com/catchorg/Catch2/blob/devel/docs/matchers.md#top
using Catch::Matchers::Contains;
using Catch::Matchers::Equals;


SCENARIO("sensormessage serialization", "[clicklib]" ) {

    GIVEN("A serialized sensormessage") {

        string sensorbinary = "../../testdata/sensormessage.bin";
        fstream input(sensorbinary, ios::in | ios::binary);
        REQUIRE(input);

        WHEN("creating sensormessage from bytes") {
            SensorMessage sensorMessage;
            REQUIRE(sensorMessage.ParseFromIstream(&input));

            THEN("it should contain the sensor values") {

                google::protobuf::Map<string, iam::SensorMessage_Object> map = sensorMessage.objects();
                auto sensors = map["robot1"].sensors();
                REQUIRE(sensors["joint1"].sensor()[0].angle() == 1.0);
                REQUIRE(sensors["joint1"].sensor()[1].anglevelocity() == 2.0);
                REQUIRE(sensors["joint1"].sensor()[2].torque() == 3.0);
                REQUIRE_THAT(sensorMessage.DebugString(), Catch::Matchers::StartsWith("messageType: SensorMessageType"));
//                REQUIRE_THAT(sensorMessage.DebugString(), Equals("SensorMessageType"));
            }
        }
    }
}
