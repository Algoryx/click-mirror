#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_all.hpp>
#include <click/ControlMessage.h>
#include <click/ControlMessageBuilder.h>
#include <click/MessageSerializer.h>
#include <Messaging.pb.h>

using namespace std;
using namespace algoryx::click;
// https://github.com/catchorg/Catch2/blob/devel/docs/matchers.md#top
using Catch::Matchers::Equals;


inline vector<double> double_vector_from(initializer_list<double> doubles) {
    return vector<double>(doubles);
}


SCENARIO("controlmessage serialization", "[clicklib]" ) {

    GIVEN("A controlmessage") {
        vector<double> angles = double_vector_from({ 1, 2, 3, 4, 5 });
        vector<double> angleVelocities = double_vector_from({ 2, 3, 4, 5, 6 });
        vector<double> torques = double_vector_from({ 3, 4, 5, 6, 7 });

        WHEN("adding three robots with controls") {
            unique_ptr<ControlMessage> controlMessage = ControlMessageBuilder::builder()
            ->object("robot1")
                    ->withAngles(angles)
                    ->withControlEvent("gripper", true)
                ->object("robot2")
                    ->withAngleVelocities(angleVelocities)
                ->object("robot3")
                    ->withTorques(torques)
            ->build();

            THEN("it should contain the control values") {
                REQUIRE(controlMessage->messageType() == ControlMessageType);
                REQUIRE(controlMessage->controlEvent("robot1", "gripper"));
                REQUIRE_THAT(controlMessage->angles("robot1"), Equals(angles));
                REQUIRE_THAT(controlMessage->angleVelocities("robot2"), Equals(angleVelocities));
                REQUIRE_THAT(controlMessage->torques("robot3"), Equals(torques));


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

                REQUIRE_THAT(controlMessage->debugString(), Equals(control_facit));
            }

            THEN("it should serialize to string") {
                MessageSerializer serializer;
                REQUIRE(serializer.serializeToString(*controlMessage).length() > 10);
            }
            
            THEN("it should be deserialized from string") {
                MessageSerializer serializer;
                string bytes = serializer.serializeToString(*controlMessage);
                
                unique_ptr<Message> message = serializer.fromBytes(bytes);
                REQUIRE(message->messageType() == ControlMessageType);
                REQUIRE_THAT(message->debugString(), Equals(controlMessage->debugString()));
            }
        }
    }
}
