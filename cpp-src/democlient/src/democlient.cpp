#include <iostream>
#include <click/Client.h>
#include <click/MessageFactory.h>
#include <click/ControlMessageBuilder.h>

using namespace std;
using namespace algoryx::click;

inline vector<double> double_vector_from(initializer_list<double> doubles) {
    return vector<double>(doubles);
}

int main(int argc, char *argv[])
{
    const string endpoint = "tcp://localhost:5555";

    // Verify protobuf version
    GOOGLE_PROTOBUF_VERIFY_VERSION;
    Client client = Client();
    cout << "Using Click client to connect to click server…" << endl;
    client.connect(endpoint);

    // Create-send-receive
    {
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
    }


    // Create-send-receive
    vector<double> angles = double_vector_from({ 1, 2, 3, 4, 5 });
    vector<double> angleVelocities = double_vector_from({ 2, 3, 4, 5, 6 });
    vector<double> torques = double_vector_from({ 3, 4, 5, 6, 7 });
    unique_ptr<ControlMessage> control_m = ControlMessageBuilder::builder()
    ->object("robot1")
            ->withAngles(angles)
            ->withControlEvent("gripper", true)
        ->object("robot2")
            ->withAngleVelocities(angleVelocities)
        ->object("robot3")
            ->withTorques(torques)
    ->build();

    cout << "Sending " << control_m->debugString() << endl;
    client.send(*control_m);
    unique_ptr<Message> response = client.blockingReceive();
    cout << "Received " << response->debugString() << endl;

    // Optional according to https://developers.google.com/protocol-buffers/docs/cpptutorial
    google::protobuf::ShutdownProtobufLibrary();
}
