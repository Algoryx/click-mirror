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

    /**
     * Asynchronously connects to an endpoint.
     * Happily keep trying to connect until there is something there.
     *
     * Inproc sockets must have a valid target already bound before connection
     * will work.
     * 
     * \param endpoint the zmq endpoint to connect to
     */
    CLICK_EXPORT void connect(const std::string& endpoint);

    /**
     * Sends bytes over the socket.
     *
     * If the socket send times out this function will return false.
     *
     * \param bytes bytes to send.
     * \return true if signal sent, false if it would have blocked or it timed out.
     */
    CLICK_EXPORT bool send(const std::string& bytes) const;

    /**
     * If there is a message ready then get it as a string
     *
     * If the socket receive times out this function will return false.
     *
     * \return true if message part received, false if it would have blocked or it timed out.
     */
    CLICK_EXPORT bool receive(std::string& responseBytes) const;

    /**
     * Sends a message over the socket.
     *
     * If the socket send times out this function will return false.
     *
     * \param message the message to send.
     * \return true if signal sent, false if it would have blocked or it timed out.
     */
    CLICK_EXPORT bool send(const Message& message) const;
    
    /**
     * Wait for the next message
     *
     * \param block whether to block while receiving or not
     * \return the received message or empty ptr if it would have blocked.
     */
    CLICK_EXPORT std::unique_ptr<Message> receive(bool block=true);

    /**
     * Wait for the next message
     *
     * \return the received message or empty ptr if it would have blocked.
     */
    CLICK_EXPORT std::unique_ptr<Message> blockingReceive();

    CLICK_EXPORT ~Client();

  private:
    std::unique_ptr<zmqpp::socket> m_socket;
    std::unique_ptr<zmqpp::context> m_context;
  };
}
