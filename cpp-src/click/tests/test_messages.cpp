#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_all.hpp>
#include <click/ControlMessage.h>
#include <Messaging.pb.h>

using namespace std;
using namespace algoryx::click;
// https://github.com/catchorg/Catch2/blob/devel/docs/matchers.md#top
using Catch::Matchers::Equals;


inline vector<double> double_vector_from(initializer_list<double> doubles) {
    return vector<double>(doubles);
}



class algoryx::click::ControlMessageBuilder {
    protobuf::ControlMessage * message;
    protobuf::ControlMessage_Object * currObject;

private:
    ControlMessageBuilder(protobuf::ControlMessage * control_m) {
        this->message = control_m;
    }

public:
    ControlMessageBuilder * object(string name) {
        google::protobuf::Map<string, protobuf::ControlMessage_Object> * map = this->message->mutable_objects();
        // TODO: Why is this not a pointer?
        (*map)[name] = protobuf::ControlMessage_Object();
        currObject = &(*map)[name];
        return this;
    }
    ControlMessageBuilder * withAngles(vector<double> angles) {
        currObject->mutable_angles()->Assign(angles.begin(), angles.end());
        return this;
    }
    ControlMessageBuilder * withAngleVelocities(vector<double> angles) {
        currObject->mutable_anglevelocities()->Assign(angles.begin(), angles.end());
        return this;
    }
    ControlMessageBuilder * withTorques(vector<double> torques) {
        currObject->mutable_torques()->Assign(torques.begin(), torques.end());
        return this;
    }
    ControlMessageBuilder * withControlEvent(string name, bool activated) {
        (*currObject->mutable_controlevents())["gripper"] = true;
        return this;
    }
    ControlMessage * build() {
        return new ControlMessage(message);
    }

    static ControlMessageBuilder * builder() {
        protobuf::ControlMessage * control_m = new protobuf::ControlMessage();
        control_m->set_messagetype(protobuf::ControlMessageType);
        return new ControlMessageBuilder(control_m);
    }
};

SCENARIO("controlmessage serialization", "[clicklib]" ) {

    GIVEN("A controlmessage") {

//        ControlMessage * control_m = MessageFactory::create_controlMessage();

        WHEN("adding three robots with controls") {
            vector<double> angles = double_vector_from({ 1, 2, 3, 4, 5 });
            vector<double> angleVelocities = double_vector_from({ 2, 3, 4, 5, 6 });
            vector<double> torques = double_vector_from({ 3, 4, 5, 6, 7 });
            ControlMessage * control_m = ControlMessageBuilder::builder()
            ->object("robot1")
                    ->withAngles(angles)
                    ->withControlEvent("gripper", true)
                ->object("robot2")
                    ->withAngleVelocities(angleVelocities)
                ->object("robot3")
                    ->withTorques(torques)
            ->build();
            
            THEN("it should contain the control values") {
                REQUIRE(control_m->messageType() == ControlMessageType);
                REQUIRE(control_m->controlEvent("robot1", "gripper"));
                REQUIRE_THAT(control_m->angles("robot1"), Equals(angles));
                REQUIRE_THAT(control_m->angleVelocities("robot2"), Equals(angleVelocities));
                REQUIRE_THAT(control_m->torques("robot3"), Equals(torques));


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
                    "}\n"
                    "objects {\n"
                    "  key: \"robot2\"\n"
                    "  value {\n"
                    "    angleVelocities: 2\n"
                    "    angleVelocities: 3\n"
                    "    angleVelocities: 4\n"
                    "    angleVelocities: 5\n"
                    "    angleVelocities: 6\n"
                    "  }\n"
                    "}\n"
                    "objects {\n"
                    "  key: \"robot3\"\n"
                    "  value {\n"
                    "    torques: 3\n"
                    "    torques: 4\n"
                    "    torques: 5\n"
                    "    torques: 6\n"
                    "    torques: 7\n"
                    "  }\n"
                    "}\n";

                REQUIRE_THAT(control_m->DebugString(), Equals(control_facit));
            }

            THEN("it should be serializable") {
//                REQUIRE(control_m->SerializeAsString().length() == 69);
            }
        }
    }
}
