#include <catch2/catch.hpp>
#include <Messaging.pb.h>
#include <click/MessageFactory.h>

using namespace std;
using namespace click::protobuf;
// https://github.com/catchorg/Catch2/blob/devel/docs/matchers.md#top
using Catch::Matchers::Equals;


SCENARIO("protobuf handshake sensorrequest message", "[click]") {
    GIVEN("A handshake sensorrequest message ") {
        SensorRequestMessage message = MessageFactory::createSensorRequestMessage();

        WHEN("serializing to bytes") {
            auto buf = message.SerializeAsString();
            THEN("it should be deserialized to origin") {
                SensorRequestMessage newmessage;
                newmessage.ParseFromString(buf);
                REQUIRE(newmessage.DebugString() == message.DebugString());
                REQUIRE(newmessage.messagetype() == message.messagetype());
            }
            THEN("it should have messagetype SensorRequest") {
                Message newmessage;
                newmessage.ParseFromString(buf);
                REQUIRE(newmessage.DebugString() == message.DebugString());
                REQUIRE(newmessage.messagetype() == message.messagetype());
            }
        }
    }

}

SCENARIO("protobuf controlmessage serialization", "[click]" ) {

    GIVEN("A controlmessage with a robot with angle controls and a gripper") {

        ControlMessage * control_m = MessageFactory::create_controlMessage();

        WHEN("adding robot controls") {
            google::protobuf::Map<string, ControlMessage_Object> * map = control_m->mutable_objects();
            (*map)["robot1"] = ControlMessage_Object();
            vector<double> angles = vector<double>({ 1, 2, 3, 4, 5 });
            (*map)["robot1"].mutable_angles()->Assign(angles.begin(), angles.end());
            (*(*map)["robot1"].mutable_controlevents())["gripper"] = true;

            THEN("it should return correct angle") {
                REQUIRE(control_m->objects().at("robot1").angles()[1] == 2.0);
                REQUIRE(control_m->objects().at("robot1").controlevents().at("gripper"));
            }

            THEN("it should contain the control values") {


                string control_facit = 
                    "messageType: ControlMessageType\n"
                    "objects {\n"
                    "  key: \"robot1\"\n"
                    "  value {\n"
                    "    angles: 1\n"
                    "    angles: 2\n"
                    "    angles: 3\n"
                    "    angles: 4\n"
                    "    angles: 5\n"
                    "    controlEvents {\n"
                    "      key: \"gripper\"\n"
                    "      value: true\n"
                    "    }\n"
                    "  }\n"
                    "}\n";

                REQUIRE_THAT(control_m->DebugString(), Equals(control_facit));
            }

            THEN("it should be serializable") {
                REQUIRE(control_m->SerializeAsString().length() == 69);
            }
        }
    }
}
