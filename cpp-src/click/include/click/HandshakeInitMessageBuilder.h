#pragma once
#include<string>
#include<memory>
#include <click/HandshakeInitMessage.h>

class click::HandshakeInitMessageBuilder {

public:
    CLICK_EXPORT std::unique_ptr<HandshakeInitMessage> build();
    CLICK_EXPORT static std::unique_ptr<HandshakeInitMessageBuilder> builder();
    CLICK_EXPORT virtual ~HandshakeInitMessageBuilder();

private:
    HandshakeInitMessageBuilder(std::unique_ptr<protobuf::HandshakeInitMessage> control_m);
    std::unique_ptr<protobuf::HandshakeInitMessage> message;
};
