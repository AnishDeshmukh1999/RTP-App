#include "ClientLayer.h"

void ClientLayer::OnAttach() {
  m_Console.AddMessage("This is the Client console!");
  const int port = 1982;
  const std::string address = "127.0.0.1";
  m_Client = std::make_unique<Networking::Client>(address, port);
  m_Client->SetLogMessageCallback(
      [this](const std::string& msg) { LogMessageCallback(msg); });
  m_Client->SetDataReceivedCallback(
      [this](const std::string& data) { DataReceivedCallback(data); });
  m_Client->Start();
}

void ClientLayer::OnDetach() { m_Client->Stop(); }

void ClientLayer::OnUIRender() { m_Console.OnUIRender(); }

void ClientLayer::LogMessageCallback(const std::string& msg) {
  m_Console.AddMessage(msg);
}

void ClientLayer::DataReceivedCallback(const std::string& data) {
  m_Console.AddMessage("Received: " + std::to_string(data.size()) + " bytes");
  // Process Data here
}
