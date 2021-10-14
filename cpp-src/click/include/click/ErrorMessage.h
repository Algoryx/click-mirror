#pragma once

#include <click/DllExport.h>
#include <click/Message.h>

#include <vector>
#include <memory>

namespace click
{
  namespace protobuf
  {
    class ErrorMessage;
  }
}

namespace click
{

  class ErrorMessageBuilder;

  class ErrorMessage : public Message
  {
  public:
    CLICK_EXPORT MessageType messageType() const;
    CLICK_EXPORT std::string message() const;
    CLICK_EXPORT std::string debugString() const;

    CLICK_EXPORT ~ErrorMessage();

  private:
    ErrorMessage(std::unique_ptr<protobuf::ErrorMessage>);
    virtual std::string serializeToBytes() const;

    std::unique_ptr<protobuf::ErrorMessage> pm;

    friend class ErrorMessageBuilder;
    friend class MessageSerializer;
  };
}
