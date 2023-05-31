#pragma once
#include<string>
#include<memory>
#include <click/HandshakeMessage.h>

class click::HandshakeMessageBuilder {

public:
    /**
     * Build the message
     *
     * \return a HandshakeMessage
     */
    CLICK_EXPORT std::unique_ptr<HandshakeMessage> build();
    /**
     * Create a builder that creates a Message
     *
     * \return a Builder
     */
    CLICK_EXPORT static std::unique_ptr<HandshakeMessageBuilder> builder();
    CLICK_EXPORT virtual ~HandshakeMessageBuilder();

private:
    HandshakeMessageBuilder(std::unique_ptr<protobuf::HandshakeMessage> control_m);
    std::unique_ptr<protobuf::HandshakeMessage> message;
};
