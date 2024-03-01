#include "Server.h"
namespace Networking {
static Server* s_instance{nullptr};
std::pair<size_t, boost::system::error_code> Server::send(
    ip::udp::socket& socket, ip::udp::endpoint& endpoint, char* packet_data,
    int len) {
  boost::system::error_code err;
  size_t sent =
      socket.send_to(boost::asio::buffer(packet_data, len), endpoint, 0, err);
  return std::make_pair(sent, err);
}

void Server::NetworkThreadFunc() {
  s_instance = this;
  m_running = true;
  if (!m_LogMessageCallback) {
    std::cout << "Log Message Callback not registered \n";
    abort();
  }

  std::thread m_networkThread;
  io_context m_io_context;
  ip::udp::socket m_socket(m_io_context);
  ip::udp::endpoint m_endpoint(ip::address::from_string(m_address), m_port);
  std::array<char, 1024> recv_buffer{};

  m_socket.open(ip::udp::v4());
  m_LogMessageCallback("Opening socket! \n");
  while (m_running) {
    m_LogMessageCallback("In the Server Run Loop!");
    std::this_thread::sleep_for(std::chrono::milliseconds(500));
  }
  m_LogMessageCallback("Closing socket! \n");
  m_socket.close();
}

void Server::Start() {
  if (m_running) return;
  m_networkThread = std::thread([this]() { NetworkThreadFunc(); });
}

void Server::Stop() { m_running = false; }

void Server::SetLogMessageCallback(const LogMessageCallback& callback) {
  m_LogMessageCallback = callback;
}

}  // namespace Networking