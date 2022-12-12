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

inline vector<double> angles = double_vector_from({0,-130.0/180.0*3.14, 30.0 / 180.0 * 3.14, 0, 0, 0,0 ,0  ,-130.0 / 180.0 * 3.14,0,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0,0,0,0,0});
inline vector<double> angleVelocities = double_vector_from({2, 3, 4, 5, 6});
inline vector<double> torques = double_vector_from({3, 4, 5, 6, 7});

std::chrono::microseconds recv_total;
std::chrono::microseconds idling_total;


unique_ptr<Message> sendReceiveBlocking(Client &client, const Message & message, bool trace = false)
{
    if (trace) {
        cout << "Sending " << message.debugString() << endl;
    }
    client.send(message);
    auto start = std::chrono::system_clock::now();
    unique_ptr<Message> response = client.blockingReceive();
    auto stop = std::chrono::system_clock::now();
    auto last_receive = stop-start;
    recv_total = std::chrono::duration_cast<std::chrono::microseconds>(last_receive);
    return response;
}

unique_ptr<Message> sendReceive(Client &client, const Message & message, bool trace = false)
{
    if (trace) {
        cout << "Sending " << message.debugString() << endl;
    }
    client.send(message);

    auto start = std::chrono::system_clock::now();
    auto recvstart = std::chrono::system_clock::now();
    while(true) {
        unique_ptr<Message> response = client.receive(false);

        if (response) {
            auto stop = std::chrono::system_clock::now();
            auto last_receive = stop-recvstart;
            auto time_btw_send_recv = recvstart-start;
            recv_total += std::chrono::duration_cast<std::chrono::microseconds>(last_receive);
            idling_total += std::chrono::duration_cast<std::chrono::microseconds>(time_btw_send_recv);
            return response;
        }
#ifndef _WIN32
        else {
            // std::this_thread::sleep_for(std::chrono::microseconds(100));
        }
#endif
        recvstart = std::chrono::system_clock::now();
    }
}

argparse::ArgumentParser parseArgs(int argc, char** argv)
{
    argparse::ArgumentParser args("my_program");
    args.add_argument("--trace")
        .help("Print sent/recv messages")
        .default_value(false)
        .implicit_value(true);
    args.add_argument("--timings")
        .help("Print timing info, e.g time spent in recv etc")
        .default_value(false)
        .implicit_value(true);
    args.add_argument("--blocking-receive")
        .help("Do blocking receives instead of non-blocking")
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
    bool timings = args.get<bool>("timings");
    bool blocking_receive = args.get<bool>("blocking-receive");
    const std::string endpoint = args.get<std::string>("addr");
    int n = args.get<int>("range");

    // Verify protobuf version
    Client client;
    unique_ptr<Message> message;
    unique_ptr<Message> reply;
    client.connect(endpoint);

    // Handshake
    message = HandshakeInitMessageBuilder::builder()->build();
    reply = sendReceiveBlocking(client, *message);

    // Controlmessage
    message = ControlMessageBuilderImpl::builder()
        ->object("mYuMi")
            ->withValues(angles)
        ->build();

    cout << "Sending "<< n << " messages" << endl;
    auto start = std::chrono::system_clock::now();
    recv_total = std::chrono::microseconds::zero();
    idling_total = std::chrono::microseconds::zero();

    for(int i=0; i<n;i++) {
        if (blocking_receive)
            reply = sendReceiveBlocking(client, *message, trace);
        else
            reply = sendReceive(client, *message, trace);
    }

    auto stop = std::chrono::system_clock::now();
    auto elapsed = stop-start;
    if (trace)
        cout << "Received " << reply->debugString() << endl;
    if (timings) {
        double tosecs = 1000000.0;
        cout << "Receive took " << recv_total.count()/tosecs << " secs in total " << recv_total.count()/tosecs/n << " per roundtrip " << endl;
        cout << "Idled betweeen send-recv for " << idling_total.count()/tosecs << " secs in total " << idling_total.count()/tosecs/n << " per roundtrip" << endl;
        cout << "Total time for " << n << " messages: " << std::chrono::duration_cast<std::chrono::microseconds>(elapsed).count()/tosecs << " secs" << endl;
    }


    // Optional according to https://developers.google.com/protocol-buffers/docs/cpptutorial
 //   google::protobuf::ShutdownProtobufLibrary();
}
