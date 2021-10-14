#pragma once
#include<string>
#include<memory>
#include <click/SensorRequestMessage.h>

class click::SensorRequestMessageBuilder {

public:
    CLICK_EXPORT std::unique_ptr<SensorRequestMessage> build();
    CLICK_EXPORT static std::unique_ptr<SensorRequestMessageBuilder> builder();
    CLICK_EXPORT virtual ~SensorRequestMessageBuilder();

private:
    SensorRequestMessageBuilder(std::unique_ptr<protobuf::SensorRequestMessage> control_m);
    std::unique_ptr<protobuf::SensorRequestMessage> message;
};
