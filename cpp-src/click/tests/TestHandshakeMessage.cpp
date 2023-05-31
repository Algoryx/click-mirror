#include <catch2/catch.hpp>
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

// TODO: Implement this fully, only build() supported yet
SCENARIO("handshake serialization", "[click]")
{
    GIVEN("A handshake message")
    {
       WHEN("it has been constructed")
        {
            unique_ptr<HandshakeMessage> message = HandshakeMessageBuilder::builder()->build();

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
