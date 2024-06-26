#include <catch2/catch_all.hpp>
#include <click/HandshakeMessage.h>
#include <click/HandshakeMessageBuilder.h>
#include <click/MessageSerializer.h>
#include <Messaging.pb.h>

using namespace std;
using namespace click;
using Catch::Matchers::Equals;
using Catch::Matchers::StartsWith;

inline vector<double> double_vector_from(initializer_list<double> doubles)
{
    return vector<double>(doubles);
}


SCENARIO("handshake serialization", "[click]")
{
    GIVEN("A handshake message")
    {
       WHEN("it has been constructed")
        {
            std::vector<std::string> controls_in_order = {"joint1", "joint2"};
            std::vector<click::ValueType> control_types_in_order = {click::ValueType::Angle, click::ValueType::AngularVelocity1D, click::ValueType::Force1D, click::ValueType::Torque1D};
            unique_ptr<HandshakeMessage> message =
                HandshakeMessageBuilderImpl::builder()
                ->withSimulationSettings({1.0})
                ->withControlType(click::ValueType::Multiple)
                ->withRobot("robot1")
                    // Inputs/Outputs:
                    ->withControlsInOrder(controls_in_order)
                    ->withControlTypesInOrder(control_types_in_order)
                    // Outputs:
                    ->withJointSensorsInOrder(controls_in_order)
                    ->withJointSensors(control_types_in_order)
                    ->withSensor("sensor1")
                        ->withTypesInOrder(control_types_in_order)
                    ->withSensor("sensor2")
                        ->withTypesInOrder(control_types_in_order)
                    ->withControlEvent("gripper1", click::ValueType::Activated)
                    ->withControlEvent("gripper2", click::ValueType::Activated)
                    ->withObjectSensors(control_types_in_order)
                ->withRobot("robot2")
                ->build();

            THEN("it should match facit")
            {
                std::string handshake_facit = R"(messageType: HandshakeMessageType
version: CURRENT_VERSION
controlType: Multiple
objects {
  key: "robot1"
  value {
    controlsInOrder: "joint1"
    controlsInOrder: "joint2"
    jointSensors: Angle
    jointSensors: AngularVelocity1D
    jointSensors: Force1D
    jointSensors: Torque1D
    controlEvents {
      key: "gripper1"
      value: Activated
    }
    controlEvents {
      key: "gripper2"
      value: Activated
    }
    sensors {
      key: "sensor1"
      value {
        types: Angle
        types: AngularVelocity1D
        types: Force1D
        types: Torque1D
      }
    }
    sensors {
      key: "sensor2"
      value {
        types: Angle
        types: AngularVelocity1D
        types: Force1D
        types: Torque1D
      }
    }
    objectSensors: Angle
    objectSensors: AngularVelocity1D
    objectSensors: Force1D
    objectSensors: Torque1D
    jointSensorsInOrder: "joint1"
    jointSensorsInOrder: "joint2"
    controlTypesInOrder: Angle
    controlTypesInOrder: AngularVelocity1D
    controlTypesInOrder: Force1D
    controlTypesInOrder: Torque1D
  }
}
objects {
  key: "robot2"
  value {
  }
}
simSettings {
  timeStep: 1
}
)";
                REQUIRE(message->debugString() == handshake_facit);
            }

            THEN("it should contain type")
            {
                REQUIRE(message->messageType() == MessageType::HandshakeMessageType);
            }

            THEN("it should have debugstring with correct type")
            {
                REQUIRE_THAT(message->debugString(), StartsWith("messageType: HandshakeMessageType"));
            }

            THEN("it should serialize to zero bytes, all defaults")
            {
                MessageSerializer serializer;
                REQUIRE(serializer.serializeToString(*message).length() > 1);
            }

            THEN("it should be deserialized from string")
            {
                MessageSerializer serializer;
                string bytes = serializer.serializeToString(*message);

                unique_ptr<Message> message = serializer.fromBytes(bytes);
                REQUIRE(message->messageType() == MessageType::HandshakeMessageType);
                REQUIRE_THAT(message->debugString(), Equals(message->debugString()));
            }
        }
    }
}
