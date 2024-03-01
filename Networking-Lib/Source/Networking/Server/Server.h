#pragma once

#include <SDKDDKVer.h>

#include <boost/asio.hpp>
#include <iostream>
#include <utility>

#define _SILENCE_STDEXT_ARR_ITERS_DEPRECATION_WARNING

using namespace boost::asio;

namespace Networking {
class Server {
 public:
  using LogMessageCallback = std::function<void(const std::string&)>;
  Server(std::string address, int port) : m_port{port}, m_address{address} {}
  ~Server() {
    if (m_networkThread.joinable()) m_networkThread.join();
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
  std::thread m_networkThread;
  std::string m_address{};
  LogMessageCallback m_LogMessageCallback{nullptr};
};
}  // namespace Networking
