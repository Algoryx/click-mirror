#pragma once
#include<string>
#include<memory>
#include <click/ResetMessage.h>

class click::ResetMessageBuilder {

public:
    /**
     * Build the message
     *
     * \return a ResetMessage
     */
    CLICK_EXPORT std::unique_ptr<ResetMessage> build();
    /**
     * Create a builder that creates a Message
     *
     * \return a Builder
     */
    CLICK_EXPORT static std::unique_ptr<ResetMessageBuilder> builder();
    CLICK_EXPORT virtual ~ResetMessageBuilder();

private:
    ResetMessageBuilder(std::unique_ptr<protobuf::ResetMessage> control_m);
    std::unique_ptr<protobuf::ResetMessage> message;
};
