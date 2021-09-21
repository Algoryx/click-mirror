#include <iostream>
#include <click/Client.h>
#include <click/MessageFactory.h>
#include <click/ControlMessageBuilder.h>
#include <click/HandshakeInitMessageBuilder.h>

using namespace std;
using namespace algoryx::click;

inline vector<double> double_vector_from(initializer_list<double> doubles) {
    return vector<double>(doubles);
}

inline vector<double> angles = double_vector_from({ 1, 2, 3, 4, 5 });
inline vector<double> angleVelocities = double_vector_from({ 2, 3, 4, 5, 6 });
inline vector<double> torques = double_vector_from({ 3, 4, 5, 6, 7 });

void sendReceiveHandshakeProtobuf(const Client &client)
{
    protobuf::HandshakeInitMessage *message = protobuf::MessageFactory::create_handshakeInitMessage();
    cout << "Type: " << message->messagetype() << endl;
    cout << "Sending " << message->DebugString() << endl;
    client.send(message->SerializeAsString());
    string responseBytes;
    client.receive(responseBytes);
    protobuf::HandshakeMessage response;
    response.ParseFromString(responseBytes);
    cout << "Received " << response.DebugString() << endl;
}

int main(int argc, char *argv[])
{
    const string endpoint = "tcp://localhost:5555";

    // Verify protobuf version
    GOOGLE_PROTOBUF_VERIFY_VERSION;
    Client client;
    cout << "Using Click client to connect to click server…" << endl;
    client.connect(endpoint);

    // Create-send-receive
    sendReceiveHandshakeProtobuf(client);

    unique_ptr<HandshakeInitMessage> hmessage = HandshakeInitMessageBuilder::builder()->build();
    cout << "Sending " << hmessage->debugString() << endl;
    client.send(*hmessage);
    unique_ptr<Message> response = client.blockingReceive();
    cout << "Received " << response->debugString() << endl;

    // Create-send-receive
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
    response = client.blockingReceive();
    cout << "Received " << response->debugString() << endl;

    // Optional according to https://developers.google.com/protocol-buffers/docs/cpptutorial
    google::protobuf::ShutdownProtobufLibrary();
}
