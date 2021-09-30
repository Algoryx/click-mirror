#pragma once

#include <click/DllExport.h>

#include <string>
#include <memory>

namespace zmqpp {
  class socket;
  class context;
}

namespace click {
  class Message;

  class Client
  {
  public:
    CLICK_EXPORT Client();

    CLICK_EXPORT void connect(const std::string& endpoint);
    CLICK_EXPORT bool send(const std::string& bytes) const;
    CLICK_EXPORT bool receive(std::string& responseBytes) const;
    CLICK_EXPORT bool send(const Message& message) const;
    CLICK_EXPORT std::unique_ptr<Message> blockingReceive();

    CLICK_EXPORT ~Client();

  private:
    std::unique_ptr<zmqpp::socket> m_socket;
    std::unique_ptr<zmqpp::context> m_context;
  };
}
