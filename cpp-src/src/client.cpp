#include <iostream>
#include <zmqpp/zmqpp.hpp>
#include "Messaging.pb.h"

using namespace std;
using namespace algoryx::click;

class MessageFactory {

    public:
        static HandshakeInitMessage * create_handshakeInitMessage() {
            HandshakeInitMessage * message = new HandshakeInitMessage();
            message->set_messagetype(HandshakeInitMessageType);
            message->set_version(CURRENT_VERSION);
            return message;
        }
};


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

    // Init protobuf
    GOOGLE_PROTOBUF_VERIFY_VERSION;
    // initialize the 0MQ context
    zmqpp::context context;

    // generate a push socket
    zmqpp::socket_type type = zmqpp::socket_type::req;
    zmqpp::socket socket(context, type);

    // open the connection
    cout << "Connecting to click server…" << endl;
    socket.connect(endpoint);

    // create message
    HandshakeInitMessage * message = MessageFactory::create_handshakeInitMessage();
    cout << "Type: " << message->messagetype() << endl;
    cout << "Sending " << message->DebugString() << endl;
    socket.send(message->SerializeAsString());
    string responseBytes;
    socket.receive(responseBytes);
    HandshakeMessage response;
    response.ParseFromString(responseBytes);
    cout << "Received " << response.DebugString() << endl;

    // Optional according to https://developers.google.com/protocol-buffers/docs/cpptutorial
    google::protobuf::ShutdownProtobufLibrary();
}
