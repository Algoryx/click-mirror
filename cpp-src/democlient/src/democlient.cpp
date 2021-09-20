#include <iostream>
#include <click/Client.h>
#include <click/MessageFactory.h>

using namespace std;
using namespace algoryx::click;

int main(int argc, char *argv[])
{
    const string endpoint = "tcp://localhost:5555";

    // Verify protobuf version
    GOOGLE_PROTOBUF_VERIFY_VERSION;
    Client client = Client();
    cout << "Using Click client to connect to click server…" << endl;
    client.connect(endpoint);

    // Create-send-receive
    using namespace protobuf;
    HandshakeInitMessage * message = MessageFactory::create_handshakeInitMessage();
    cout << "Type: " << message->messagetype() << endl;
    cout << "Sending " << message->DebugString() << endl;
    client.send(message->SerializeAsString());
    string responseBytes;
    client.receive(responseBytes);
    HandshakeMessage response;
    response.ParseFromString(responseBytes);
    cout << "Received " << response.DebugString() << endl;

    // Optional according to https://developers.google.com/protocol-buffers/docs/cpptutorial
    google::protobuf::ShutdownProtobufLibrary();
}
