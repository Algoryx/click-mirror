#if defined(_WIN32)
#include <io.h> // For _mktemp on Windows
#else
#include <unistd.h> // For mkstemp on Unix-like systems
#endif
#include <catch2/catch_all.hpp>
#include <click/Server.h>
#include <click/Client.h>
#include <click/ControlMessageBuilder.h>
#include <click/HandshakeInitMessageBuilder.h>
#include <click/HandshakeMessageBuilder.h>


using namespace std;
using namespace click;


SCENARIO("Client-Server integration test", "[click]")
{
#if defined(_WIN32)
    CATCH_SKIP("Skipping Client-Server integration test on Windows, ipc is not supported there.");
#endif
    GIVEN("A client and a server")
    {
        WHEN("Sending a HandshakeInitMessage")
        {
            Client client;
            Server server;
            unique_ptr<Message> server_message;
            char tmp_filename[] = "/tmp/click_test_XXXXXX";
#if defined(_WIN32)
            _mktemp(tmp_filename); // Windows-specific temporary file creation
#else
            int fd = mkstemp(tmp_filename); // Secure temporary file creation
            close(fd); // Close the file descriptor immediately
#endif
            std::string endpoint = std::string("ipc://") + tmp_filename;
            client.connect(endpoint);
            server.bind(endpoint);

            unique_ptr<Message> client_message = HandshakeInitMessageBuilder::builder()->build();
            client.send(*client_message);
            unique_ptr<Message> server_reply = server.receive(true);
            server.send(*HandshakeMessageBuilderImpl::builder()->build());
            unique_ptr<Message> client_reply = client.receive(true);

            THEN("Handshake shoule be returned")
            {
                REQUIRE(client_reply->messageType() == MessageType::HandshakeMessageType);
            }
        }
    }
}
