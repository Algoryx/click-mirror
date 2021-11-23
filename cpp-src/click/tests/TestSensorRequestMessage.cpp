#include <catch2/catch.hpp>
#include <click/SensorRequestMessage.h>
#include <click/SensorRequestMessageBuilder.h>
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

SCENARIO("SensorRequestMessage serialization", "[click]")
{
    GIVEN("A handshake init message")
    {
       WHEN("it has been constructed")
        {
            unique_ptr<SensorRequestMessage> message = SensorRequestMessageBuilder::builder()->build();

            THEN("it should contain type")
            {
                REQUIRE(message->messageType() == MessageType::SensorRequestMessageType);
            }

            THEN("it should have debugstring without type since it is the default")
            {
                REQUIRE_THAT(message->debugString(), StartsWith("messageType: SensorRequestMessageType"));
            }

            THEN("it should serialize to at least 1 byte")
            {
                MessageSerializer serializer;
                REQUIRE(serializer.serializeToString(*message).length() > 1);
            }

            THEN("it should be deserialized from string")
            {
                MessageSerializer serializer;
                string bytes = serializer.serializeToString(*message);
                unique_ptr<Message> smessage = serializer.fromBytes(bytes);
                REQUIRE(smessage->messageType() == MessageType::SensorRequestMessageType);
                REQUIRE_THAT(smessage->debugString(), Equals(message->debugString()));
            }
        }
    }
}
