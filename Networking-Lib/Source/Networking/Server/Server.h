#pragma once

#include <SDKDDKVer.h>

#include <boost/asio.hpp>
#include <iostream>
#include <utility>

#include "Networking/RTP/RTP.h"

using namespace boost::asio;

namespace Networking {
class Server {
 public:
  using LogMessageCallback = std::function<void(const std::string&)>;
  Server(std::string address, int port) : m_port{port}, m_address{address} {}
  ~Server() {
    if (m_NetworkThread.joinable()) m_NetworkThread.join();
  }

  std::pair<size_t, boost::system::error_code> send(ip::udp::socket& socket,
                                                    ip::udp::endpoint& endpoint,
                                                    char* packet_data, int len);
  void NetworkThreadFunc();
  void Start();
  void Stop();
  void SetLogMessageCallback(const LogMessageCallback& callback);

 private:
  bool m_running{};
  int m_port{};
  std::thread m_NetworkThread;
  std::string m_address{};
  LogMessageCallback m_LogMessageCallback{nullptr};
};
}  // namespace Networking
