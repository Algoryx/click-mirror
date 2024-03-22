#pragma once
#include<string>
#include<memory>
#include <click/ResetMessage.h>

class CLICK_EXPORT click::ResetMessageBuilder {

public:
    /**
     * Build the message
     *
     * \return a ResetMessage
     */
    std::unique_ptr<ResetMessage> build();
    /**
     * Create a builder that creates a Message
     *
     * \return a Builder
     */
    static std::unique_ptr<ResetMessageBuilder> builder();
    virtual ~ResetMessageBuilder();

private:
    ResetMessageBuilder(std::unique_ptr<protobuf::ResetMessage> control_m);
    std::unique_ptr<protobuf::ResetMessage> message;
};
