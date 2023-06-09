#include <memory>
#include <Messaging.pb.h>
#include <click/HandshakeInitMessageBuilder.h>

using namespace click;
using namespace std;

HandshakeInitMessageBuilder::HandshakeInitMessageBuilder(unique_ptr<protobuf::HandshakeInitMessage> pm)
{
    this->message = std::move(pm);
}

unique_ptr<HandshakeInitMessage> HandshakeInitMessageBuilder::build()
{
    return unique_ptr<HandshakeInitMessage>(new HandshakeInitMessage(std::move(message)));
}

unique_ptr<HandshakeInitMessageBuilder> HandshakeInitMessageBuilder::builder()
{
    unique_ptr<protobuf::HandshakeInitMessage> pm = make_unique<protobuf::HandshakeInitMessage>();
    pm->set_messagetype(protobuf::HandshakeInitMessageType);
    pm->set_version(protobuf::CURRENT_VERSION);
    return unique_ptr<HandshakeInitMessageBuilder>(new HandshakeInitMessageBuilder(std::move(pm)));
}

HandshakeInitMessageBuilder::~HandshakeInitMessageBuilder() = default;
