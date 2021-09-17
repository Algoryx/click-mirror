#include <iostream>
#include <click/Client.h>
#include <click/MessageFactory.h>

using namespace std;
using namespace algoryx::click;


void controlmessage()
{
    ControlMessage *control_m = new ControlMessage();
    control_m->set_messagetype(ControlMessageType);
    google::protobuf::Map<string, ControlMessage_Object> *map = control_m->mutable_objects();
    (*map)["robot1"] = ControlMessage_Object();

    double src[] = {1, 2, 3, 4, 5};
    int n = sizeof(src) / sizeof(src[0]);
    std::vector<double> data(src, src + n);
    (*map)["robot1"].mutable_anglevelocities()->Assign(data.begin(), data.end());
    (*(*map)["robot1"].mutable_controlevents())["gripper"] = true;
    cout << control_m->DebugString();
    cout << control_m->SerializeAsString().length();
    cout << endl;
}

//  Hello World client
#include <iostream>

using namespace std;

int main(int argc, char *argv[])
{
    const string endpoint = "tcp://localhost:5555";

    // Verify protobuf version
    GOOGLE_PROTOBUF_VERIFY_VERSION;
    Client client = Client();
    cout << "Using Click client to connect to click server…" << endl;
    client.connect(endpoint);

    // Create-send-receive
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
