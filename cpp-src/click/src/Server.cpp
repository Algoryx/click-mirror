#include <click/Server.h>
#include <Messaging.pb.h>
#include <click/Message.h>
#include <click/ControlMessage.h>
#include <click/SensorMessage.h>
#include <click/MessageSerializer.h>

#pragma warning(push)
#pragma warning(disable : 4005) // Macro redefinition in zmqpp
#include <zmqpp/zmqpp.hpp>
#pragma warning(pop)

#include <memory>
#include "Server.h"

using namespace click;
using namespace std;

Server::Server()
{
  // Verify protobuf version
  GOOGLE_PROTOBUF_VERIFY_VERSION;
  m_context = std::make_unique<zmqpp::context>();

  zmqpp::socket_type type = zmqpp::socket_type::rep;
  m_socket = std::make_unique<zmqpp::socket>(*m_context, type);
}

void Server::bind(const std::string& endpoint) {
  m_socket->bind(endpoint);
}

bool Server::send(const std::string& bytes) {
  bool success = m_socket->send(bytes);
  if (success)
    m_send_is_next_action = false;
  return success;
}

bool Server::receive_bytes(std::string& responseBytes) {
  bool success = m_socket->receive(responseBytes, true);
  if (success)
    m_send_is_next_action = true;
  return success;
}

bool Server::send(const Message& message) {
  MessageSerializer serializer;
  string bytes = serializer.serializeToString(message);
  bool success = m_socket->send(bytes);
  if (success)
    m_send_is_next_action = false;
  return success;
}

bool click::Server::can_send()
{
  return m_send_is_next_action;
}

bool click::Server::can_recv()
{
  return !m_send_is_next_action;
}

unique_ptr<Message> Server::receive(bool block)
{
  MessageSerializer serializer;
  std::string bytes;
  bool status = m_socket->receive(bytes, !block);
  if (status) {
    m_send_is_next_action = true;
    return serializer.fromBytes(bytes);
  }
  else
    return unique_ptr<Message>();
}

unique_ptr<Message> Server::blocking_receive()
{
  return this->receive(true);
}

void Server::terminate()
{
  if (m_socket)
  {
    m_socket->close();
    m_socket.reset();
  }
  if (m_context)
  {
    m_context->terminate();
    m_context.reset();
  }
}

Server::~Server()
{
  this->terminate();
}
