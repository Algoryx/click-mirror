#include <chrono>
#include <thread>
#include <iostream>
#include <click/Client.h>
#include <click/ControlMessageBuilder.h>
#include <click/HandshakeInitMessageBuilder.h>

using namespace std;
using namespace click;

inline vector<double> double_vector_from(initializer_list<double> doubles)
{
    return vector<double>(doubles);
}

inline vector<double> angles = double_vector_from({1, 2, 3, 4, 5});
inline vector<double> angleVelocities = double_vector_from({2, 3, 4, 5, 6});
inline vector<double> torques = double_vector_from({3, 4, 5, 6, 7});


unique_ptr<Message> sendReceiveBlocking(Client &client, unique_ptr<Message> message)
{
    cout << "Sending " << message->debugString() << endl;
    client.send(*message);
    unique_ptr<Message> response = client.blockingReceive();
    cout << "Received " << response->debugString() << endl;
    return response;
}

unique_ptr<Message> sendReceive(Client &client, unique_ptr<Message> message)
{
    cout << "Sending " << message->debugString() << endl;
    client.send(*message);
    while(true)
    {
        unique_ptr<Message> response = client.receive(false);
        if (response)
        {
            cout << "Received " << response->debugString() << endl;
            return response;
        }
        else
        {
            cout << "Would have blocked, trying again" << endl;
            using namespace std::this_thread;
            using namespace std::chrono;
            sleep_for(microseconds(100));
        }
    }
}

int main(int argc, char *argv[])
{
    const string endpoint = "tcp://localhost:5555";
//    const string endpoint = "tcp://host.docker.internal:5555";

    // Verify protobuf version
    Client client;
    unique_ptr<Message> message;
    unique_ptr<Message> reply;
    client.connect(endpoint);

    // Handshake
    message = HandshakeInitMessageBuilder::builder()->build();
    reply = sendReceiveBlocking(client, move(message));

    // Controlmessage
    message = ControlMessageBuilderImpl::builder()
        ->object("robot1")
            ->withAngles(angles)
            ->withControlEvent("gripper", true)
        ->object("robot2")
            ->withAngleVelocities(angleVelocities)
        ->object("robot3")
            ->withTorques(torques)
        ->build();

    reply = sendReceive(client, move(message));

    // Optional according to https://developers.google.com/protocol-buffers/docs/cpptutorial
 //   google::protobuf::ShutdownProtobufLibrary();
}
