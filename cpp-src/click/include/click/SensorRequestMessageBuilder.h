#pragma once
#include<string>
#include<memory>
#include <click/SensorRequestMessage.h>

class CLICK_EXPORT click::SensorRequestMessageBuilder {

public:
    /**
     * Build the message
     *
     * \return a SensorRequestMessage
     */
    std::unique_ptr<SensorRequestMessage> build();
    /**
     * Create a builder that creates a Message
     *
     * \return a Builder
     */
    static std::unique_ptr<SensorRequestMessageBuilder> builder();
    virtual ~SensorRequestMessageBuilder();

private:
    SensorRequestMessageBuilder(std::unique_ptr<protobuf::SensorRequestMessage> control_m);
    std::unique_ptr<protobuf::SensorRequestMessage> message;
};
