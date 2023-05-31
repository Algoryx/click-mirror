#include <memory>
#include <Messaging.pb.h>
#include <click/ResetMessageBuilder.h>

using namespace click;
using namespace std;

ResetMessageBuilder::ResetMessageBuilder(unique_ptr<protobuf::ResetMessage> pm)
{
    this->message = std::move(pm);
}

unique_ptr<ResetMessage> ResetMessageBuilder::build()
{
    return unique_ptr<ResetMessage>(new ResetMessage(std::move(message)));
}

unique_ptr<ResetMessageBuilder> ResetMessageBuilder::builder()
{
    unique_ptr<protobuf::ResetMessage> pm = make_unique<protobuf::ResetMessage>();
    pm->set_messagetype(protobuf::ResetMessageType);
    return unique_ptr<ResetMessageBuilder>(new ResetMessageBuilder(std::move(pm)));
}

ResetMessageBuilder::~ResetMessageBuilder() = default;
