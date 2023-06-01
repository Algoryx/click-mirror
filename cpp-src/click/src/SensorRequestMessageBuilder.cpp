#include <memory>
#include <Messaging.pb.h>
#include <click/SensorRequestMessageBuilder.h>

using namespace click;
using namespace std;

SensorRequestMessageBuilder::SensorRequestMessageBuilder(unique_ptr<protobuf::SensorRequestMessage> pm)
{
    this->message = std::move(pm);
}

unique_ptr<SensorRequestMessage> SensorRequestMessageBuilder::build()
{
    return unique_ptr<SensorRequestMessage>(new SensorRequestMessage(std::move(message)));
}

unique_ptr<SensorRequestMessageBuilder> SensorRequestMessageBuilder::builder()
{
    unique_ptr<protobuf::SensorRequestMessage> pm = make_unique<protobuf::SensorRequestMessage>();
    pm->set_messagetype(protobuf::SensorRequestMessageType);
    return unique_ptr<SensorRequestMessageBuilder>(new SensorRequestMessageBuilder(std::move(pm)));
}

SensorRequestMessageBuilder::~SensorRequestMessageBuilder() = default;
