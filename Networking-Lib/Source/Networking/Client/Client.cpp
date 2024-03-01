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
  while (m_Running) {
    m_LogMessageCallback("In the Client Run Loop!");
    std::this_thread::sleep_for(std::chrono::milliseconds(500));
  }
  m_LogMessageCallback("Closing socket! \n");
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
}  // namespace Networking