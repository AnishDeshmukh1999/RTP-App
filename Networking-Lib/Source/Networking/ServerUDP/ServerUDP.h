#pragma once

#include <SDKDDKVer.h>

#include <boost/asio.hpp>
#include <iostream>
#include <thread>
#include <utility>

#include "Networking/RTP/RTP.h"

using namespace boost::asio;

namespace Networking {
class ServerUDP {
 public:
  using LogMessageCallback = std::function<void(const std::string&)>;
  using DataAccessCallback = std::function<void()>;
  ServerUDP(std::string address, int port) : m_port{port}, m_address{address} {
    ip::udp::endpoint endpoint(ip::address::from_string(m_address), m_port);
    std::array<char, 1024> recv_buffer{};
  }
  ~ServerUDP() {
    if (m_NetworkThread.joinable()) m_NetworkThread.join();
  }

  std::pair<size_t, boost::system::error_code> send(ip::udp::socket& socket,
                                                    ip::udp::endpoint& endpoint,
                                                    const char* packet_data,
                                                    int len);
  void NetworkThreadFunc();
  void Start();
  void Stop();
  void SetLogMessageCallback(const LogMessageCallback& callback);

 private:
  bool m_running{};
  int m_port{};
  std::thread m_NetworkThread;
  std::string m_address{};
  io_context io_context;
  ip::udp::socket socket{io_context};
  LogMessageCallback m_LogMessageCallback{nullptr};
};
}  // namespace Networking
