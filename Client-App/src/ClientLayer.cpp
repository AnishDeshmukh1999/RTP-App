#include "ClientLayer.h"

void ClientLayer::OnAttach() {
  m_Console.AddMessage("This is the Client console!");

  // Connect to the UDP Server later
  m_ClientUDP = std::make_unique<Networking::ClientUDP>(m_address, m_port);

  m_ClientUDP->SetLogMessageCallback(
      [this](const std::string& msg) { LogMessageCallback(msg); });
  m_ClientUDP->SetDataReceivedCallback(
      [this](const std::string& data) { DataReceivedCallback(data); });
  // m_ClientUDP->Start();

  m_ClientTCP = std::make_unique<Networking::ClientTCP>();
}

void ClientLayer::OnDetach() {
  // m_ClientUDP->Stop();
}

void ClientLayer::OnUIRender() {
  if (!isTCPConnected()) {
    UI_ConnectionModal();
  } else {
    ConnectedUIRender();
  }
  m_Console.OnUIRender();
}

void ClientLayer::UI_ConnectionModal() {
  if (!m_ConnectionModalOpen && m_ClientTCP->GetConnectionStatus() !=
                                    Networking::ConnectionStatus::Connected) {
    ImGui::OpenPopup("Connect to server");
  }
  m_ConnectionModalOpen = ImGui::BeginPopupModal(
      "Connect to server", nullptr, ImGuiWindowFlags_AlwaysAutoResize);

  if (m_ConnectionModalOpen) {
    ImGui::Text("Please Connect to the Server");
    char addressArr[128] = "127.0.0.1";
    char portArr[10] = "1892";
    ImGui::InputTextWithHint("Server Address", "127.0.0.1", addressArr,
                             IM_ARRAYSIZE(addressArr));
    ImGui::InputTextWithHint("Server Port", "1892", portArr,
                             IM_ARRAYSIZE(portArr));
    std::string address = addressArr;
    std::string port = portArr;
    if (ImGui::Button("Connect")) {
      int k = 01;
      if (Networking::Util::ValidateAddress(address.c_str())) {
        m_ClientTCP->ConnectToServer(address, port);
      }
    }
    ImGui::EndPopup();
  }
}
void ClientLayer::LogMessageCallback(const std::string& msg) {
  m_Console.AddMessage(msg);
}

void ClientLayer::ConnectedUIRender() {
  ImGui::Begin("Disconnect Menu", NULL,
               ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize);
  if (ImGui::Button("Disconnect")) {
    m_ClientTCP->Disconnect();
  }
  ImGui::End();
}

bool ClientLayer::isTCPConnected() {
  if (m_ClientTCP->GetConnectionStatus() ==
      Networking::ConnectionStatus::Connected) {
    return true;
  }
  return false;
}

void ClientLayer::DataReceivedCallback(const std::string& data) {
  m_Console.AddMessage("Received: " + std::to_string(data.size()) + " bytes");
  // Process Data here
}
