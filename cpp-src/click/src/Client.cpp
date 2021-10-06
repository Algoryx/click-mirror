#include <click/Client.h>
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

using namespace click;
using namespace std;

Client::Client()
{
  // Verify protobuf version
  GOOGLE_PROTOBUF_VERIFY_VERSION;
  m_context = std::make_unique<zmqpp::context>();

  zmqpp::socket_type type = zmqpp::socket_type::request;
  m_socket = std::make_unique<zmqpp::socket>(*m_context, type);
}

void Client::connect(const std::string& endpoint) {
  m_socket->connect(endpoint);
}

bool Client::send(const std::string& bytes) const {
  return m_socket->send(bytes);
}

bool Client::receive(std::string& responseBytes) const{
  return m_socket->receive(responseBytes);
}

bool Client::send(const Message& message) const
{
  MessageSerializer serializer;
  string bytes = serializer.serializeToString(message);
  return m_socket->send(bytes);
}

unique_ptr<Message> Client::blockingReceive()
{
  MessageSerializer serializer;
  string bytes;
  bool status = m_socket->receive(bytes);
  return serializer.fromBytes(bytes);
}

Client::~Client()
{
  m_socket->close();
  m_socket.reset();
}
