#include <memory>
#include <Messaging.pb.h>
#include <click/ErrorMessageBuilder.h>

using namespace click;
using namespace std;

ErrorMessageBuilder::ErrorMessageBuilder(unique_ptr<protobuf::ErrorMessage> pm)
{
    this->message = std::move(pm);
}

CLICK_EXPORT ErrorMessageBuilder* ErrorMessageBuilder::withMessage(const std::string& errormessage){
    this->message->set_errormessage(errormessage);
    return this;
}

unique_ptr<ErrorMessage> ErrorMessageBuilder::build()
{
    return unique_ptr<ErrorMessage>(new ErrorMessage(std::move(message)));
}

unique_ptr<ErrorMessageBuilder> ErrorMessageBuilder::builder()
{
    unique_ptr<protobuf::ErrorMessage> pm = make_unique<protobuf::ErrorMessage>();
    pm->set_messagetype(protobuf::ErrorMessageType);
    return unique_ptr<ErrorMessageBuilder>(new ErrorMessageBuilder(std::move(pm)));
}

ErrorMessageBuilder::~ErrorMessageBuilder() = default;
