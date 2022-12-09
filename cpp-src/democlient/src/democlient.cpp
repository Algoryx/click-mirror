#include <chrono>
#include <thread>
#include <iostream>
#include <argparse/argparse.hpp>
#include <click/Client.h>
#include <click/ControlMessageBuilder.h>
#include <click/HandshakeInitMessageBuilder.h>

using namespace std;
using namespace click;

inline vector<double> double_vector_from(initializer_list<double> doubles)
{
    return vector<double>(doubles);
}

inline vector<double> angles = double_vector_from({1, 2});
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

unique_ptr<Message> sendReceive(Client &client, const Message & message, bool trace = false)
{
    if (trace)
    {
        cout << "Sending " << message.debugString() << endl;
    }
    client.send(message);

    int slept = 0;

    while(true)
    {
        unique_ptr<Message> response = client.receive(false);

        if (response && slept)
        {
            cout << "Would have blocked " << slept << " microseconds" << endl;
        }

        if (response)
        {
            return response;
        }
#ifndef _WIN32
        else
        {
            using namespace std::this_thread;
            using namespace std::chrono;
            auto start = std::chrono::system_clock::now();
            sleep_for(microseconds(100));
            auto stop = std::chrono::system_clock::now();
            std::chrono::duration<double> elapsed_seconds = stop-start;
            slept += elapsed_seconds.count() * 1000000;
        }
#endif
    }
}

argparse::ArgumentParser parseArgs(int argc, char** argv)
{
    argparse::ArgumentParser args("my_program");
    args.add_argument("--trace")
        .help("Print sent/recv messages")
        .default_value(false)
        .implicit_value(true);
      args.add_argument("--addr")
        .help("An alternate address to connect to, like ipc:///tmp/click.ipc")
        .default_value(std::string("tcp://localhost:5555"));
      args.add_argument("--range")
        .help("How many extra messages to send/recv")
        .default_value(0)
        .scan<'i', int>();
    args.parse_args(argc, argv);
    return args;
}

int main(int argc, char *argv[])
{
    auto args = parseArgs(argc, argv);
    bool trace = args.get<bool>("trace");
    const std::string endpoint = args.get<std::string>("addr");
    int n = args.get<int>("range");

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
            ->withAngles(angles)
        ->build();

    cout << "Sending "<< n << " messages" << endl;
    for(int i=0; i<n;i++)
        reply = sendReceive(client, *message, trace);
        // reply = sendReceiveBlocking(client, *message, false);

    cout << "Received " << reply->debugString() << endl;

    // Optional according to https://developers.google.com/protocol-buffers/docs/cpptutorial
 //   google::protobuf::ShutdownProtobufLibrary();
}
