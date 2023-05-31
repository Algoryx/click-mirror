#include <chrono>
#include <thread>
#include <iostream>
#include <argparse/argparse.hpp>
#include <click/Server.h>
#include <click/ControlMessageBuilder.h>
#include <click/SensorMessage.h>
#include <click/SensorMessageBuilder.h>
#include <click/ErrorMessageBuilder.h>
#include <click/HandshakeMessageBuilder.h>
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

std::unique_ptr<Message> sendReceive(Server& server, const Message& message, bool trace = false)
{
    if (trace) {
        cout << "Sending " << message.debugString() << endl;
    }
    server.send(message);

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
#ifndef _WIN32
        else {
            // Uncomment next line to lower unecessary CPU usage, but be wary that it increases communication latency
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
    args.add_argument("--blocking-receive")
        .help("Do blocking receives instead of non-blocking")
        .default_value(false)
        .implicit_value(true);
      args.add_argument("--addr")
        .help("An alternate address to bind to, like ipc:///tmp/click.ipc")
        .default_value(std::string("tcp://*:5555"));
    args.parse_args(argc, argv);
    return args;
}


std::unique_ptr<SensorMessage> sensor_message() {
    size_t size = 2;
    vector<double> values;
    for(int i=0; i<size; i++)
        values.push_back(1.0);

    return SensorMessageBuilderImpl::builder()
        ->object("robot1")
            ->withAngles(values)
            ->withAngleVelocities(values)
            ->withTorques(values)
        ->object("box")
            ->withPosition({1.0, 2.0, 3.0})
        ->build();

    // box = sensor_m.objects["box"]
    // sensor = box.objectSensors.add()
    // sensor.position.arr.extend([1.0, 2.0, 3.0])
    // sensor = box.objectSensors.add()
    // sensor.rpy.arr.extend([4.0, 5.0, 6.0])
    // val = robot.sensors["external_1"].sensor.add()
    // val.force.arr.extend([4.0, 4.1, 4.2])
    // val = robot.sensors["external_1"].sensor.add()
    // val.angularAcceleration.arr.extend([5.0, 5.1, 5.2])

    // return sensor_m    
}

int main(int argc, char *argv[])
{
    auto args = parseArgs(argc, argv);
    bool trace = args.get<bool>("trace");
    bool blocking_receive = args.get<bool>("blocking-receive");
    const std::string endpoint = args.get<std::string>("addr");

    // Verify protobuf version
    Server server;
    std::unique_ptr<Message> message;
    std::unique_ptr<Message> reply;
    server.bind(endpoint);

    std::unique_ptr<ErrorMessage> error_message = ErrorMessageBuilder::builder()->build();

    while(true) {
        if (blocking_receive)
            reply = receiveBlocking(server, trace);
        else
            reply = sendReceive(server, *message, trace);
        switch(reply->messageType()) {
            case MessageType::HandshakeInitMessageType:
                if(trace)
                    std::cerr <<  "Got handshakeinit message: " << std::endl;
                server.send(*HandshakeMessageBuilder::builder()->build());
                break;
            case MessageType::ControlMessageType:
                if(trace)
                    std::cerr <<  "Got control message: " << std::endl;
                server.send(*error_message);
                break;
            default:
                if(trace)
                    std::cerr <<  "Got unhandled message: " << std::endl;
                server.send(*ErrorMessageBuilder::builder()->build());
        }
    }

    // Optional according to https://developers.google.com/protocol-buffers/docs/cpptutorial
 //   google::protobuf::ShutdownProtobufLibrary();
}
