#include <memory>
#include <Messaging.pb.h>
#include <click/HandshakeMessageBuilder.h>

using namespace click;
using namespace std;

HandshakeMessageBuilder::HandshakeMessageBuilder(unique_ptr<protobuf::HandshakeMessage> pm)
{
    this->message = std::move(pm);
}

unique_ptr<HandshakeMessage> HandshakeMessageBuilder::build()
{
    return unique_ptr<HandshakeMessage>(new HandshakeMessage(std::move(message)));
}

unique_ptr<HandshakeMessageBuilder> HandshakeMessageBuilder::builder()
{
    unique_ptr<protobuf::HandshakeMessage> pm = make_unique<protobuf::HandshakeMessage>();
    pm->set_messagetype(protobuf::HandshakeMessageType);
    pm->set_version(protobuf::CURRENT_VERSION);
    return unique_ptr<HandshakeMessageBuilder>(new HandshakeMessageBuilder(std::move(pm)));
}

HandshakeMessageBuilder::~HandshakeMessageBuilder() = default;
