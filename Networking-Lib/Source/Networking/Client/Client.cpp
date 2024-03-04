#include "Client.h"

namespace Networking {
static Client* s_Instance = nullptr;

void Client::NetworkThreadFunc() {
  if (!m_LogMessageCallback) {
    std::cout << "Log Message Callback not registered \n";
    abort();
  }
  if (!m_DataReceivedCallback) {
    std::cout << "Data Received Callback not registered \n";
    abort();
  }
  s_Instance = this;
  m_Running = true;

  m_socket.open(ip::udp::v4());
  m_socket.bind(ip::udp::endpoint(ip::address::from_string(m_address), m_port));

  wait(m_error);
  m_LogMessageCallback("Starting IO Context\n");
  while (m_Running) {
    m_io_context.run_one();
  }
  m_LogMessageCallback("Receiver Exit \n");
}
void Client::Start() {
  if (m_Running) return;
  m_NetworkThread = std::thread([this]() { NetworkThreadFunc(); });
}
void Client::Stop() {
  m_Running = false;

  if (m_NetworkThread.joinable()) m_NetworkThread.join();
}
void Client::SetLogMessageCallback(const LogMessageCallback& callback) {
  m_LogMessageCallback = callback;
}
void Client::SetDataReceivedCallback(const DataReceivedCallback& callback) {
  m_DataReceivedCallback = callback;
}
void Client::handle_receive(const boost::system::error_code& ec,
                            std::size_t length,
                            boost::system::error_code* out_ec,
                            std::size_t* out_length) {
  if (ec) {
    m_LogMessageCallback("Receive Failed: " + ec.message() + "\n");
    if (ec == boost::asio::error::operation_aborted) {
      return;
    }
  } else {
    m_DataReceivedCallback(
        std::string(recv_buffer.begin(), recv_buffer.begin() + length));
  }
  *out_ec = ec;
  *out_length = length;
  wait(*out_ec);
}
void Client::wait(boost::system::error_code& error) {
  deadline_.expires_from_now(seconds(10));

  // Set up the variables that receive the result of the asynchronous
  // operation. The error code is set to would_block to signal that the
  // operation is incomplete. Asio guarantees that its asynchronous
  // operations will never fail with would_block, so any other value in
  // ec indicates completion.
  error = boost::asio::error::would_block;
  std::size_t length = 0;

  m_socket.async_receive_from(
      boost::asio::buffer(recv_buffer), m_endpoint,
      boost::bind(&Client::handle_receive, this, boost::placeholders::_1,
                  boost::placeholders::_2, &error, &length));
}
}  // namespace Networking