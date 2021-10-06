#pragma once
#include<string>
#include<memory>
#include <click/HandshakeInitMessage.h>

class click::HandshakeInitMessageBuilder {

public:
    std::unique_ptr<HandshakeInitMessage> build();
    static std::unique_ptr<HandshakeInitMessageBuilder> builder();
    virtual ~HandshakeInitMessageBuilder();

private:
    HandshakeInitMessageBuilder(std::unique_ptr<protobuf::HandshakeInitMessage> control_m);
    std::unique_ptr<protobuf::HandshakeInitMessage> message;
};
