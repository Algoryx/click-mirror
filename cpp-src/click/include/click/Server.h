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
 * A Click Server
 *
 * NOTE: Should not be declared static in Windows, @see Server#terminate
 *
 */
class CLICK_EXPORT Server
{
  public:
    Server(size_t bufsize = 32768);

    /**
     * Bind server to an endpoint so that client can connect to it.
     *
     * \param endpoint The zmq endpoint to connect to
     */
    virtual void bind(const std::string& endpoint);

    /**
     * Sends bytes over the socket.
     *
     * If the socket send times out this function will return false.
     *
     * \param bytes Bytes to send.
     * \return true if signal sent, false if it would have blocked or it timed out.
     */
    virtual bool send(const std::string& bytes);

    /**
     * If there is a message ready then get it as a string
     *
     * If the socket receive times out this function will return false.
     *
     * \return true if message part received, false if it would have blocked or it timed out.
     */
    virtual bool receive_bytes(std::string& responseBytes);

    /**
     * Sends a message over the socket.
     *
     * If the socket send times out this function will return false.
     *
     * \param message The message to send.
     * \return true if signal sent, false if it would have blocked or it timed out.
     */
    virtual bool send(const Message& message);

    /**
     * @brief Since caller must interleave send/recv, this convenience method reports which of send/recv can be done now.
     *
     * \return true if ok to send, false otw
     */
    virtual bool must_send();

    /**
     * @brief Since caller must interleave send/recv, this convenience method reports which of send/recv can be done now.
     *
     * \return true if ok to recv, false otw
     */
    virtual bool must_recv();

    /**
     * Wait for the next message
     *
     * \param block Whether to block while receiving or not
     * \return the received message or empty ptr if it would have blocked.
     */
    virtual std::unique_ptr<Message> receive(bool block=false);

    /**
     * Wait for the next message
     *
     * \return the received message or empty ptr if it would have blocked.
     */
    virtual std::unique_ptr<Message> blocking_receive();

    /**
     * Shutdown ZMQ Sockets and Context
     * Any subsequent calls to connect, send and/or receive will fail!
     *
     * NOTE: Calling terminate() is done automatically in destructor
     * The reason this method is exposed is if Server is declared as static in a Windows application,
     * because if static the destructor will be called after Windows Sockets are shut down causing a "WSASTARTUP not yet performed" assertion
     */
    virtual void terminate();

    virtual ~Server();

  private:
    std::unique_ptr<zmq::socket_t> m_socket;
    std::unique_ptr<zmq::context_t> m_context;
    bool m_send_is_next_action = false;
    size_t m_bufsize;
  };
}
