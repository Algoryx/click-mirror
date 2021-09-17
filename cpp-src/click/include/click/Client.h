#pragma once

#include <click/DllExport.h>

#include <string>
#include <memory>

namespace zmqpp {
  class socket;
  class context;
}

namespace algoryx { namespace click {
  // class Message;
  // class ControlMessage;
  // class SensorMessage;
  class MessageSerializer;

  class Client
  {
  public:
    CLICK_EXPORT Client();
    CLICK_EXPORT Client(std::unique_ptr<MessageSerializer>&& handler);

    CLICK_EXPORT void connect(const std::string& endpoint);
//    CLICK_EXPORT bool send(const ControlMessage& message) const;
//    CLICK_EXPORT bool send(const SensorMessage& message) const;
//    CLICK_EXPORT bool receive(Message& message);
    CLICK_EXPORT bool send(const std::string& bytes) const;
    CLICK_EXPORT bool receive(std::string& responseBytes);

    CLICK_EXPORT ~Client();

  private:
    std::unique_ptr<zmqpp::socket> m_socket;
    std::unique_ptr<zmqpp::context> m_context;
    std::unique_ptr<MessageSerializer> m_serializer;
  };
}}
