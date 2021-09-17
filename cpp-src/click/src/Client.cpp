#include <click/Client.h>
#include <click/Message.h>
#include <click/ControlMessage.h>
#include <click/SensorMessage.h>
#include <click/MessageSerializer.h>

#pragma warning(push)
#pragma warning(disable : 4005) // Macro redefinition in zmqpp
#include <zmqpp/zmqpp.hpp>
#pragma warning(pop)

#include <memory>

using namespace algoryx::click;

/*
algoryx::click::Client::Client(std::unique_ptr<MessageSerializer>&& serializer)
{

  // initialize the 0MQ context
  m_context = std::make_unique<zmqpp::context>();

  // generate a push socket
  zmqpp::socket_type type = zmqpp::socket_type::request;
  m_socket = std::make_unique<zmqpp::socket>(*m_context, type);

  m_handler = std::move(handler);
}
*/
Client::Client()
{
  m_context = std::make_unique<zmqpp::context>();

  zmqpp::socket_type type = zmqpp::socket_type::request;
  m_socket = std::make_unique<zmqpp::socket>(*m_context, type);
}

void Client::connect(const std::string& endpoint)
{
  m_socket->connect(endpoint);
}

    // CLICK_EXPORT bool Client::send(const std::string& bytes) const;
    // CLICK_EXPORT bool Client::receive(std::string& responseBytes);

bool Client::send(const std::string& bytes) const
{
  return m_socket->send(bytes);
}

bool Client::receive(std::string& responseBytes)
{
  return m_socket->receive(responseBytes);
}
/*
bool Client::send(const ControlMessage& message) const
{
  auto ss = m_handler->encodeMessage(message);
  return m_socket->send(ss);
}

bool Client::send(const SensorMessage& message) const
{
  auto ss = m_handler->encodeMessage(message);
  return m_socket->send(ss);
}

bool Client::receive(MessageBase& message)
{
  zmqpp::message z_message;
  bool status = m_socket->receive(z_message);
  std::cout << "Received: " << z_message.parts() << " parts" << std::endl;
  std::string p = z_message.get(0);
  m_handler->decodeMessage(p, message);
  return status;
}
*/
Client::~Client()
{
  // TODO: Understand when destructor is called - enable explicit call?
  m_socket->close();
  m_socket.reset();
}
