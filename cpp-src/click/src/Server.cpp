#include <memory>
#include <zmq.hpp>
#include <click/Server.h>
#include <Messaging.pb.h>
#include <click/Message.h>
#include <click/ControlMessage.h>
#include <click/SensorMessage.h>
#include <click/MessageSerializer.h>

using namespace click;
using namespace std;

Server::Server()
{
  // Verify protobuf version
  GOOGLE_PROTOBUF_VERIFY_VERSION;
  m_context = std::make_unique<zmq::context_t>();

  zmq::socket_type type = zmq::socket_type::rep;
  m_socket = std::make_unique<zmq::socket_t>(*m_context, type);
}

void Server::bind(const std::string& endpoint) {
  m_socket->bind(endpoint);
}

bool Server::send(const std::string& bytes) {
  bool success = m_socket->send(zmq::buffer(std::string_view(bytes))).has_value();
  if (success)
    m_send_is_next_action = false;
  return success;
}

bool Server::receive_bytes(std::string& responseBytes) {
  zmq::message_t msg;
  auto res = m_socket->recv(msg, zmq::recv_flags::dontwait);
  responseBytes.copy(static_cast<char*>(msg.data()), msg.size());
  if (res.has_value())
    m_send_is_next_action = true;
  return res.has_value();
}

bool Server::send(const Message& message) {
  MessageSerializer serializer;
  string bytes = serializer.serializeToString(message);
  bool success = m_socket->send(zmq::buffer(std::string_view(bytes))).has_value();
  if (success)
    m_send_is_next_action = false;
  return success;
}

bool click::Server::must_send()
{
  return m_send_is_next_action;
}

bool click::Server::must_recv()
{
  return !m_send_is_next_action;
}

unique_ptr<Message> Server::receive(bool block)
{
  auto recv_flags = zmq::recv_flags::dontwait;
  if (block)
    recv_flags = zmq::recv_flags::none;

  zmq::message_t msg;
  auto status = m_socket->recv(msg, recv_flags);

  if (status.has_value()) {
    MessageSerializer serializer;
    m_send_is_next_action = true;
    std::string bytes(static_cast<char*>(msg.data()), msg.size());
    return serializer.fromBytes(bytes);
  }
  else {
    return unique_ptr<Message>();
  }
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
    m_context->shutdown();
    m_context->close();
    m_context.reset();
  }
}

Server::~Server()
{
  this->terminate();
}
