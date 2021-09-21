#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_all.hpp>
#include <click/HandshakeInitMessage.h>
#include <click/HandshakeInitMessageBuilder.h>
#include <click/MessageSerializer.h>
#include <Messaging.pb.h>

using namespace std;
using namespace algoryx::click;
// https://github.com/catchorg/Catch2/blob/devel/docs/matchers.md#top
using Catch::Matchers::Equals;


inline vector<double> double_vector_from(initializer_list<double> doubles) {
    return vector<double>(doubles);
}


SCENARIO("handshakeInit serialization", "[clicklib]" ) {

    GIVEN("A handshake init message") {

        WHEN("it has been constructed") {
            unique_ptr<HandshakeInitMessage> HandshakeInitMessage = HandshakeInitMessageBuilder::builder()->build();

            THEN("it should contain type") {
                REQUIRE(HandshakeInitMessage->messageType() == HandshakeInitMessageType);
            }

            THEN("it should show empty debug string since it is all defaults") {

                REQUIRE_THAT(HandshakeInitMessage->debugString(), Equals(""));
            }

            THEN("it should serialize to zero bytes, all defaults") {
                MessageSerializer serializer;
                REQUIRE(serializer.serializeToString(*HandshakeInitMessage).length() == 0);
            }
            
            THEN("it should be deserialized from string") {
                MessageSerializer serializer;
                string bytes = serializer.serializeToString(*HandshakeInitMessage);
                
                unique_ptr<Message> message = serializer.fromBytes(bytes);
                REQUIRE(message->messageType() == HandshakeInitMessageType);
                REQUIRE_THAT(message->debugString(), Equals(HandshakeInitMessage->debugString()));
            }
        }
    }
}
