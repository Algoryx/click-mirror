#pragma once
#include<string>
#include<memory>
#include <click/ErrorMessage.h>

class click::ErrorMessageBuilder {

public:
    CLICK_EXPORT std::unique_ptr<ErrorMessage> build();
    CLICK_EXPORT static std::unique_ptr<ErrorMessageBuilder> builder();
    CLICK_EXPORT ErrorMessageBuilder * withMessage(const std::string &);
    CLICK_EXPORT virtual ~ErrorMessageBuilder();

private:
    ErrorMessageBuilder(std::unique_ptr<protobuf::ErrorMessage> control_m);
    std::unique_ptr<protobuf::ErrorMessage> message;
};
