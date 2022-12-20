#pragma once

#include <click/DllExport.h>

#include <string>
#include <memory>

namespace zmq {
  class socket_t;
  class context_t;
}

namespace click {
  class Message;

/**
 * A Click Client that can be used to connect to a Click Server.
 * 
 * NOTE: Should not be declared static in Windows, @see Client#terminate
 * 
 */
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
     * \param endpoint The zmq endpoint to connect to
     */
    CLICK_EXPORT void connect(const std::string& endpoint);

    /**
     * Sends bytes over the socket.
     *
     * If the socket send times out this function will return false.
     *
     * \param bytes Bytes to send.
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
     * \param message The message to send.
     * \return true if signal sent, false if it would have blocked or it timed out.
     */
    CLICK_EXPORT bool send(const Message& message) const;
    
    /**
     * Wait for the next message
     *
     * \param block Whether to block while receiving or not
     * \return the received message or empty ptr if it would have blocked.
     */
    CLICK_EXPORT std::unique_ptr<Message> receive(bool block=true);

    /**
     * Wait for the next message
     *
     * \return the received message or empty ptr if it would have blocked.
     */
    CLICK_EXPORT std::unique_ptr<Message> blockingReceive();

    /**
     * Shutdown ZMQ Sockets and Context
     * Any subsequent calls to connect, send and/or receive will fail!
     * 
     * NOTE: Calling terminate() is done automatically in destructor
     * The reason this method is exposed is if Client is declared as static in a Windows application,
     * because if static the destructor will be called after Windows Sockets are shut down causing a "WSASTARTUP not yet performed" assertion
     */
    CLICK_EXPORT void terminate();

    CLICK_EXPORT ~Client();

  private:
    std::unique_ptr<zmq::socket_t> m_socket;
    std::unique_ptr<zmq::context_t> m_context;
  };
}
