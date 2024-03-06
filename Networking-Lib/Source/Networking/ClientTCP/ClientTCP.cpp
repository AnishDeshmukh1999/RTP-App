#include "ClientTCP.h"

namespace Networking {
static ClientTCP* s_Instance = nullptr;
void ClientTCP::ConnectToServer(std::string& address, std::string& port) {
  /*if (m_Running) return;
  if (m_NetworkThread.joinable()) m_NetworkThread.join();*/
  m_ConnectionStatus = Networking::ConnectionStatus::Connected;
}
void ClientTCP::Disconnect() {
  m_Running = false;

  // if (m_NetworkThread.joinable()) m_NetworkThread.join();
  m_ConnectionStatus = Networking::ConnectionStatus::Disconnected;
}

void ClientTCP::NetworkThreadFunc() {
  s_Instance = this;
  m_ConnectionStatus = Networking::ConnectionStatus::Connecting;
}

}  // namespace Networking
