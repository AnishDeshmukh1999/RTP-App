#pragma once
#include <boost/asio.hpp>
#include <iostream>
#include <thread>
#include <utility>

#include "Walnut/Core/Buffer.h"

namespace Networking {
class Client {
 public:
  using LogMessageCallback = std::function<void(const std::string&)>;
  using DataReceivedCallback = std::function<void(const std::string&)>;
  Client(std::string address, int port) : m_address{address}, m_port{port} {};
  ~Client() {
    if (m_NetworkThread.joinable()) m_NetworkThread.join();
  };

  void NetworkThreadFunc();
  void Start();
  void Stop();
  void SetLogMessageCallback(const LogMessageCallback& callback);
  void SetDataReceivedCallback(const DataReceivedCallback& callback);

 private:
  bool m_Running{};
  int m_port{};
  std::thread m_NetworkThread;
  std::string m_address{};
  LogMessageCallback m_LogMessageCallback{nullptr};
  DataReceivedCallback m_DataReceivedCallback{nullptr};
};
}  // namespace Networking
