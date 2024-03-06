#pragma once
#include <functional>
#include <string>
#include <thread>
namespace Networking {
enum class ConnectionStatus {
  Disconnected = 0,
  Connected,
  Connecting,
  FailedToConnect
};
class ClientTCP {
 public:
  ClientTCP() = default;
  ~ClientTCP() {}
  using DataReceivedCallback = std::function<void(std::string& str)>;
  ConnectionStatus GetConnectionStatus() const { return m_ConnectionStatus; }
  void ConnectToServer(std::string& address, std::string& port);
  void Disconnect();

 private:
  void NetworkThreadFunc();
  std::thread m_NetworkThread;
  ConnectionStatus m_ConnectionStatus = ConnectionStatus::Disconnected;
  DataReceivedCallback m_DataReceivedCallback;
  bool m_Running = false;
};
}  // namespace Networking
