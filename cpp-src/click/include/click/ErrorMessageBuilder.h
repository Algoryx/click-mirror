#pragma once
#include<string>
#include<memory>
#include <click/ErrorMessage.h>

class CLICK_EXPORT click::ErrorMessageBuilder {

public:
    /**
     * Build the message
     *
     * \return a ErrorMessage
     */
    std::unique_ptr<ErrorMessage> build();
    /**
     * Create a builder that creates a Message
     *
     * \return a Builder
     */
    static std::unique_ptr<ErrorMessageBuilder> builder();
    /**
     * Add informative text to the error message
     *
     * \return a Builder
     */
    ErrorMessageBuilder * withMessage(const std::string& );
    virtual ~ErrorMessageBuilder();

private:
    ErrorMessageBuilder(std::unique_ptr<protobuf::ErrorMessage> control_m);
    std::unique_ptr<protobuf::ErrorMessage> message;
};
