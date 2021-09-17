#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_all.hpp>
#include <Messaging.pb.h>
#include <click/MessageFactory.h>

using namespace std;
using namespace algoryx::click;
// https://github.com/catchorg/Catch2/blob/devel/docs/matchers.md#top
using Catch::Matchers::Equals;


vector<double> double_vector_with(initializer_list<double> doubles) {
    return vector<double>(doubles);
}

SCENARIO("controlmessage serialization", "[clicklib]" ) {

    GIVEN("A controlmessage with a robot with angle controls and a gripper") {

        ControlMessage * control_m = MessageFactory::create_controlMessage();

        WHEN("adding robot controls") {
            google::protobuf::Map<string, ControlMessage_Object> * map = control_m->mutable_objects();
            (*map)["robot1"] = ControlMessage_Object();
            vector<double> angles = double_vector_with({ 1, 2, 3, 4, 5 });
            (*map)["robot1"].mutable_anglevelocities()->Assign(angles.begin(), angles.end());
            (*(*map)["robot1"].mutable_controlevents())["gripper"] = true;

            THEN("it should contain the control values") {

                string control_facit = 
                    "messageType: ControlMessageType\n"
                    "objects {\n"
                    "  key: \"robot1\"\n"
                    "  value {\n"
                    "    angleVelocities: 1\n"
                    "    angleVelocities: 2\n"
                    "    angleVelocities: 3\n"
                    "    angleVelocities: 4\n"
                    "    angleVelocities: 5\n"
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
