#pragma once
#include<string>
#include<memory>
#include <click/HandshakeInitMessage.h>

class CLICK_EXPORT click::HandshakeInitMessageBuilder {

public:
    /**
     * Build the message
     *
     * \return a HandshakeInitMessage
     */
    std::unique_ptr<HandshakeInitMessage> build();
    /**
     * Create a builder that creates a Message
     *
     * \return a Builder
     */
    static std::unique_ptr<HandshakeInitMessageBuilder> builder();
    virtual ~HandshakeInitMessageBuilder();

private:
    HandshakeInitMessageBuilder(std::unique_ptr<protobuf::HandshakeInitMessage> control_m);
    std::unique_ptr<protobuf::HandshakeInitMessage> message;
};
