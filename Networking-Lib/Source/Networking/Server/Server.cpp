#include "Server.h"
namespace Networking {
static Server* s_instance{nullptr};
std::pair<size_t, boost::system::error_code> Server::send(
    ip::udp::socket& socket, ip::udp::endpoint& endpoint,
    const char* packet_data, int len) {
  std::vector<char> encodedRTPData = RTP::encodeToRTP(packet_data, len);

  boost::system::error_code err;
  size_t sent =
      socket.send_to(boost::asio::buffer(encodedRTPData), endpoint, 0, err);
  return std::make_pair(sent, err);
}

void Server::NetworkThreadFunc() {
  s_instance = this;
  m_running = true;
  if (!m_LogMessageCallback) {
    std::cout << "Log Message Callback not registered \n";
    abort();
  }

  io_context io_context;
  ip::udp::socket socket(io_context);
  ip::udp::endpoint endpoint(ip::address::from_string(m_address), m_port);
  std::array<char, 1024> recv_buffer{};

  socket.open(ip::udp::v4());
  m_LogMessageCallback("Opening socket! \n");
  int i = 0;
  while (m_running) {
    // m_LogMessageCallback("In the Server Run Loop!");
    std::string in = "Hello: " + std::to_string(i++) + "\n";
    m_LogMessageCallback("Sending Message: " + in);
    auto res = Server::send(socket, endpoint, in.c_str(), in.length());
    m_LogMessageCallback("Sent: " + std::to_string(res.first) +
                         " bytes, err: " + res.second.message());
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));
  }
  m_LogMessageCallback("Closing socket! \n");
  socket.close();
}

void Server::Start() {
  if (m_running) return;
  m_NetworkThread = std::thread([this]() { NetworkThreadFunc(); });
}

void Server::Stop() { m_running = false; }

void Server::SetLogMessageCallback(const LogMessageCallback& callback) {
  m_LogMessageCallback = callback;
}

}  // namespace Networking