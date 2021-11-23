#pragma once
#include<string>
#include<memory>
#include <click/ErrorMessage.h>

class click::ErrorMessageBuilder {

public:
    /**
     * Build the message
     *
     * \return a ErrorMessage
     */
    CLICK_EXPORT std::unique_ptr<ErrorMessage> build();
    /**
     * Create a builder that creates a Message
     *
     * \return a Builder
     */
    CLICK_EXPORT static std::unique_ptr<ErrorMessageBuilder> builder();
    /**
     * Add informative text to the error message
     *
     * \return a Builder
     */
    CLICK_EXPORT ErrorMessageBuilder * withMessage(const std::string &);
    CLICK_EXPORT virtual ~ErrorMessageBuilder();

private:
    ErrorMessageBuilder(std::unique_ptr<protobuf::ErrorMessage> control_m);
    std::unique_ptr<protobuf::ErrorMessage> message;
};
