#include <memory>
#include <zmq.hpp>
#include <click/Client.h>
#include <Messaging.pb.h>
#include <click/Message.h>
#include <click/ControlMessage.h>
#include <click/SensorMessage.h>
#include <click/MessageSerializer.h>

using namespace click;
using namespace std;

Client::Client()
{
  // Verify protobuf version
  GOOGLE_PROTOBUF_VERIFY_VERSION;
  m_context = std::make_unique<zmq::context_t>();

  zmq::socket_type type = zmq::socket_type::req;
  m_socket = std::make_unique<zmq::socket_t>(*m_context, type);
}

void Client::connect(const std::string& endpoint) {
  m_socket->connect(endpoint);
}

bool Client::send(const std::string& bytes) const {
  return m_socket->send(zmq::buffer(std::string_view(bytes))).has_value();
}

bool Client::receive(std::string& responseBytes) const{
  zmq::mutable_buffer buf;
  auto res = m_socket->recv(buf, zmq::recv_flags::dontwait);
  responseBytes.copy(static_cast<char*>(buf.data()), buf.size());
  return res.has_value();
}

bool Client::send(const Message& message) const
{
  MessageSerializer serializer;
  string bytes = serializer.serializeToString(message);
  return m_socket->send(zmq::buffer(std::string_view(bytes))).has_value();
}

unique_ptr<Message> Client::receive(bool block)
{
  auto recv_flags = zmq::recv_flags::dontwait;
  if (block)
    recv_flags = zmq::recv_flags::none;

  zmq::mutable_buffer buf;
  auto status = m_socket->recv(buf, recv_flags);

  if (status.has_value()) {
    string bytes;
    bytes.copy(static_cast<char*>(buf.data()), buf.size());
    MessageSerializer serializer;
    return serializer.fromBytes(bytes);
  }
  else
    return unique_ptr<Message>();
}

unique_ptr<Message> Client::blockingReceive()
{
  return this->receive();
}

void Client::terminate()
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

Client::~Client()
{
  this->terminate();
}
