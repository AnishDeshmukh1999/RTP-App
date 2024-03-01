#include "ServerLayer.h"

void ServerLayer::OnAttach() {
  m_Console.AddMessage("This is the console!");

  const int port = 1982;
  const std::string address = "127.0.0.1";

  m_Server = std::make_unique<Networking::Server>(address, port);
  m_Server->SetLogMessageCallback(
      [this](const std::string& msg) { LogMessageCallback(msg); });
  m_Server->Start();
}

void ServerLayer::OnDetach() { m_Server->Stop(); }

void ServerLayer::OnUpdate(float ts) {}

void ServerLayer::OnUIRender() { m_Console.OnUIRender(); }

void ServerLayer::LogMessageCallback(const std::string& msg) {
  m_Console.AddMessage(msg);
}
