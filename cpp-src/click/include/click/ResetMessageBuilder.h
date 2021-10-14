#pragma once
#include<string>
#include<memory>
#include <click/ResetMessage.h>

class click::ResetMessageBuilder {

public:
    CLICK_EXPORT std::unique_ptr<ResetMessage> build();
    CLICK_EXPORT static std::unique_ptr<ResetMessageBuilder> builder();
    CLICK_EXPORT virtual ~ResetMessageBuilder();

private:
    ResetMessageBuilder(std::unique_ptr<protobuf::ResetMessage> control_m);
    std::unique_ptr<protobuf::ResetMessage> message;
};
