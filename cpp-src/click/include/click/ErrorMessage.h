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

  class CLICK_EXPORT ErrorMessage : public Message
  {
  public:
    MessageType messageType() const;
    std::string message() const;
    std::string debugString() const;

    ~ErrorMessage();

  private:
    ErrorMessage(std::unique_ptr<protobuf::ErrorMessage>);
    virtual std::string serializeToBytes() const;

    std::unique_ptr<protobuf::ErrorMessage> pm;

    friend class ErrorMessageBuilder;
    friend class MessageSerializer;
  };
}
