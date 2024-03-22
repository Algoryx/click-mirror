#include <catch2/catch_all.hpp>
#include <click/ErrorMessage.h>
#include <click/ErrorMessageBuilder.h>
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

SCENARIO("ErrorMessage serialization", "[click]")
{
    GIVEN("A handshake init message")
    {
       WHEN("it has been constructed")
        {
            unique_ptr<ErrorMessage> message = ErrorMessageBuilder::builder()
                ->withMessage("a very bad thing happened")
                ->build();

            THEN("it should contain type")
            {
                REQUIRE(message->messageType() == MessageType::ErrorMessageType);
            }

            THEN("it should have debugstring with correct type")
            {
                REQUIRE_THAT(message->debugString(), StartsWith("messageType: ErrorMessageType"));
            }

            THEN("it should have message")
            {
                REQUIRE_THAT(message->message(), Equals("a very bad thing happened"));
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
                REQUIRE(smessage->messageType() == MessageType::ErrorMessageType);
                REQUIRE_THAT(smessage->debugString(), Equals(message->debugString()));
            }
        }
    }
}
