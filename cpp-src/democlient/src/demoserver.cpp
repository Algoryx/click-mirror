#include <chrono>
#include <thread>
#include <iostream>
#include <argparse/argparse.hpp>
#include <click/ControlMessageBuilder.h>
#include <click/ErrorMessageBuilder.h>
#include <click/HandshakeMessageBuilder.h>
#include <click/HandshakeInitMessageBuilder.h>
#include <click/MessageSerializer.h>
#include <click/SensorMessage.h>
#include <click/SensorMessageBuilder.h>
#include <click/Server.h>

using namespace std;
using namespace click;

inline vector<double> double_vector_from(initializer_list<double> doubles)
{
    return vector<double>(doubles);
}

inline vector<double> angles = double_vector_from({1, 2});
inline vector<double> angleVelocities = double_vector_from({2, 3, 4, 5, 6});
inline vector<double> torques = double_vector_from({3, 4, 5, 6, 7});

std::chrono::microseconds recv_total;
std::chrono::microseconds idling_total;


std::unique_ptr<Message> receiveBlocking(Server& server, bool trace = false)
{
    if (trace) {
        cerr << "Receive blocking " << endl;
    }
    auto start = std::chrono::system_clock::now();
    std::unique_ptr<Message> response = server.blockingReceive();
    auto stop = std::chrono::system_clock::now();
    auto last_receive = stop-start;
    recv_total = std::chrono::duration_cast<std::chrono::microseconds>(last_receive);
    return response;
}

std::unique_ptr<Message> receive(Server& server, bool trace = false)
{
    auto start = std::chrono::system_clock::now();
    auto recvstart = std::chrono::system_clock::now();
    while(true) {
        std::unique_ptr<Message> response = server.receive(false);

        if (response) {
            auto stop = std::chrono::system_clock::now();
            auto last_receive = stop-recvstart;
            auto time_btw_send_recv = recvstart-start;
            recv_total += std::chrono::duration_cast<std::chrono::microseconds>(last_receive);
            idling_total += std::chrono::duration_cast<std::chrono::microseconds>(time_btw_send_recv);
            return response;
        }
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
    args.add_argument("--blocking-receive")
        .help("Do blocking receives instead of non-blocking")
        .default_value(false)
        .implicit_value(true);
    args.add_argument("--addr")
        .help("An alternate address to bind to, like ipc:///tmp/click.ipc")
        .default_value(std::string("tcp://*:5555"));
    args.add_argument("--trace-sizes")
        .help("print size of what is sent/received")
        .default_value(false)
        .implicit_value(true);

    args.parse_args(argc, argv);
    return args;
}


std::unique_ptr<SensorMessage> build_sensor_message() {
    size_t size = 2;
    vector<double> values;
    for(int i=0; i<size; i++)
        values.push_back(1.0);

    return SensorMessageBuilderImpl::builder()
        ->object("robot1")
            ->withAngles(values)
            ->withAngleVelocities(values)
            ->withTorques(values)
            ->withSensor("external_1")
                ->withForce({4, 4.1, 4.2})
                ->withAngularAcceleration({5, 5.1, 5.2})
        ->object("box")
            ->withPosition({1.0, 2.0, 3.0})
            ->withRPY({4.0, 5.0, 6.0})
        ->build();
}

int main(int argc, char *argv[])
{
    auto args = parseArgs(argc, argv);
    bool trace = args.get<bool>("trace");
    bool blocking_receive = args.get<bool>("blocking-receive");
    const std::string endpoint = args.get<std::string>("addr");
    bool trace_sizes = args.get<bool>("trace-sizes");

    // Verify protobuf version
    Server server;
    std::unique_ptr<Message> message;
    std::unique_ptr<Message> reply;
    server.bind(endpoint);

    auto sensor_message = build_sensor_message();
    if (trace_sizes) {
        MessageSerializer message_serializer;
        std::cerr << "Sensor message size is " << message_serializer.serializeToString(*sensor_message).size() << std::endl;
    }

    while(true) {
        if (blocking_receive)
            reply = receiveBlocking(server, trace);
        else
            reply = receive(server, trace);
        switch(reply->messageType()) {
            case MessageType::HandshakeInitMessageType:
                if(trace)
                    std::cerr <<  "Got handshakeinit message: " << std::endl;
                server.send(*HandshakeMessageBuilder::builder()->build());
                break;
            case MessageType::ControlMessageType: {
                if(trace)
                    std::cerr <<  "Got control message: " << std::endl;
                server.send(*sensor_message);
                break;
            }
            default:
                if(trace)
                    std::cerr <<  "Got unhandled message: " << std::endl;
                server.send(*ErrorMessageBuilder::builder()->build());
        }
    }

    // Optional according to https://developers.google.com/protocol-buffers/docs/cpptutorial
 //   google::protobuf::ShutdownProtobufLibrary();
}
