#include <catch2/catch_all.hpp>
#include <click/HandshakeInitMessage.h>
#include <click/HandshakeInitMessageBuilder.h>
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

SCENARIO("handshakeInit serialization", "[click]")
{
    GIVEN("A handshake init message")
    {
       WHEN("it has been constructed")
        {
            unique_ptr<HandshakeInitMessage> HandshakeInitMessage = HandshakeInitMessageBuilder::builder()->build();

            THEN("it should contain type")
            {
                REQUIRE(HandshakeInitMessage->messageType() == MessageType::HandshakeInitMessageType);
            }

            THEN("it should have debugstring without type since it is the default")
            {
                REQUIRE_THAT(HandshakeInitMessage->debugString(), StartsWith("version: CURRENT_VERSION"));
            }

            THEN("it should serialize to zero bytes, all defaults")
            {
                MessageSerializer serializer;
                REQUIRE(serializer.serializeToString(*HandshakeInitMessage).length() > 1);
            }

            THEN("it should be deserialized from string")
            {
                MessageSerializer serializer;
                string bytes = serializer.serializeToString(*HandshakeInitMessage);

                unique_ptr<Message> message = serializer.fromBytes(bytes);
                REQUIRE(message->messageType() == MessageType::HandshakeInitMessageType);
                REQUIRE_THAT(message->debugString(), Equals(HandshakeInitMessage->debugString()));
            }
        }
    }
}
