#include "ClientLayer.h"

#include <Walnut/ApplicationGUI.h>
#include <Walnut/UI/UI.h>

void ClientLayer::OnAttach() {
  m_AlbumArtPtr = std::make_unique<Walnut::Image>(m_DefaultImagePath);
  m_Console.AddMessage("This is the Client console!");

  m_Client = std::make_unique<Walnut::Client>();
  m_Client->SetServerConnectedCallback([this]() { OnConnected(); });
  m_Client->SetServerDisconnectedCallback([this]() { OnDisconnected(); });
  m_Client->SetDataReceivedCallback(
      [this](const Walnut::Buffer data) { OnDataReceived(data); });

  // Connect to the UDP Server later
  m_ClientUDP = std::make_unique<Networking::ClientUDP>(m_address, m_port);

  m_ClientUDP->SetLogMessageCallback(
      [this](const std::string& msg) { LogMessageCallback(msg); });
  m_ClientUDP->SetDataReceivedCallback(
      [this](const std::string& data) { DataReceivedCallbackUDP(data); });
  // m_ClientUDP->Start();
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

void ClientLayer::OnConnected() {
  LogMessageCallback("Connected to Server");
  Message::Request request;
  request.set_type(Message::Request::GET_ID3_TAG);
  Walnut::Buffer buffer;
  std::string requestSerialized = request.SerializeAsString();
  size_t size = requestSerialized.size();
  buffer.Allocate(size + sizeof(size_t));
  Walnut::BufferStreamWriter streamWriter(buffer);
  streamWriter.WriteString(requestSerialized);
  m_Client->SendBuffer(streamWriter.GetBuffer());
}

void ClientLayer::OnDisconnected() {
  LogMessageCallback("Lost Connection to Server");
}

void ClientLayer::OnDataReceived(const Walnut::Buffer buffer) {
  if (isTCPConnected()) {
    LogMessageCallback("Received Response");

    Message::Response response;
    Walnut::BufferStreamReader responseStreamReader(buffer);
    std::string responseAsString;

    bool success = responseStreamReader.ReadString(responseAsString);
    WL_CORE_VERIFY(success);
    if (!success) {
      LogMessageCallback("Failed to parse Response");
      return;
    }

    response.ParseFromString(responseAsString);

    if (response.has_id3v2tag()) {
      Message::ID3v2Tag tag = response.id3v2tag();
      m_CurrentPlayingTag = MP3::MP3::getTagFields(tag);
      m_CurrentPlayingImage = MP3::MP3::getImageDataAsString(tag);
      uint32_t w, h;
      void* data = Walnut::Image::Decode(m_CurrentPlayingImage.c_str(),
                                         m_CurrentPlayingImage.length(), w, h);
      m_AlbumArtPtr = std::unique_ptr<Walnut::Image>(
          new Walnut::Image(w, h, Walnut::ImageFormat::RGBA, data));
    } else if (response.has_response()) {
    } else {
      LogMessageCallback("Invalid Response type");
      return;
    }
  } else {
    LogMessageCallback("Received Response but client is not Connected");
  }
}

void ClientLayer::UI_ConnectionModal() {
  ImVec2 screenSize = ImGui::GetIO().DisplaySize;
  ImVec2 mainWindowPos = ImGui::GetMainViewport()->Pos;
  if (!m_ConnectionModalOpen && !isTCPConnected()) {
    ImGui::OpenPopup("Connect to server");
  }

  ImGui::SetNextWindowPos(ImVec2(mainWindowPos.x, mainWindowPos.y),
                          ImGuiCond_Always);
  ImGui::SetNextWindowSize(ImVec2(screenSize.x, screenSize.y), 0);
  m_ConnectionModalOpen = ImGui::BeginPopupModal("Connect to server", nullptr,
                                                 ImGuiWindowFlags_NoResize);

  if (m_ConnectionModalOpen) {
    ImGui::Text("Please Connect to the Server");
    char addressArr[128] = "127.0.0.1";
    char portArr[10] = "1892";
    ImGui::InputTextWithHint("Addr", "Address", addressArr,
                             IM_ARRAYSIZE(addressArr));
    ImGui::InputTextWithHint("port", "Port", portArr, IM_ARRAYSIZE(portArr));
    std::string address = std::string(addressArr) + ":" + std::string(portArr);
    if (ImGui::Button("Connect")) {
      if (Networking::Util::ValidateAddress(address.c_str())) {
        m_Client->ConnectToServer(address);
      }
    }
    if (Walnut::UI::ButtonCentered("Quit")) Walnut::Application::Get().Close();

    if (isTCPConnected()) {
      LogMessageCallback("Client Connected");
    } else if (m_Client->GetConnectionStatus() ==
               Walnut::Client::ConnectionStatus::FailedToConnect) {
      ImGui::TextColored(ImVec4(0.9f, 0.2f, 0.1f, 1.0f), "Connection failed.");
      const auto& debugMessage = m_Client->GetConnectionDebugMessage();
      if (!debugMessage.empty())
        ImGui::TextColored(ImVec4(0.9f, 0.2f, 0.1f, 1.0f),
                           debugMessage.c_str());
    } else if (m_Client->GetConnectionStatus() ==
               Walnut::Client::ConnectionStatus::Connecting) {
      ImGui::TextColored(ImVec4(0.8f, 0.8f, 0.8f, 1.0f), "Connecting...");
    }

    ImGui::EndPopup();
  }
}

void ClientLayer::LogMessageCallback(const std::string& msg) {
  m_Console.AddMessage(msg);
}

ImVec1 CalculateTextStart(ImVec1 centerPos, const std::string& txt) {
  ImVec2 textSize = ImGui::CalcTextSize(txt.c_str());
  return ImVec1(centerPos.x - textSize.x * 0.5f);
}

ImVec1 CalculateButtonStart(ImVec1 centerPos, ImVec1 buttonWidth) {
  return ImVec1(centerPos.x - buttonWidth.x * 0.5f);
}

void ClientLayer::ConnectedUIRender() {
  ImVec2 mainWindowPos = ImGui::GetMainViewport()->Pos;
  // ImGui::SetNextWindowPos(ImVec2(mainWindowPos.x + 50, mainWindowPos.y +
  // 50)); ImGui::SetNextWindowSize(ImVec2(50, 50), ImGuiCond_FirstUseEver);
  // ImGui::Begin("Disconnect Menu", NULL,
  //              ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize |
  //                  ImGuiWindowFlags_NoCollapse);
  // if (ImGui::Button("Disconnect")) {
  //   m_Client->Disconnect();
  // }
  // ImGui::End();
  ImVec2 nextWindowPosVec = ImVec2(mainWindowPos.x, mainWindowPos.y);
  ImGui::SetNextWindowPos(nextWindowPosVec);
  ImGui::SetNextWindowSize(ImVec2(330, 500));
  ImGui::Begin("Play Window", NULL,
               ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize |
                   ImGuiWindowFlags_NoTitleBar);
  ImVec2 imageLeftCorner =
      ImVec2(nextWindowPosVec.x + 50, nextWindowPosVec.y + 50);
  ImVec2 imageRightCorner =
      ImVec2(imageLeftCorner.x + m_AlbumArtPtr->GetWidth() / 2,
             imageLeftCorner.y + m_AlbumArtPtr->GetHeight() / 2);
  ImGui::Image(m_AlbumArtPtr->GetDescriptorSet(), ImVec2(300, 300));
  ImVec1 windowCenterWidth = ImGui::GetWindowSize().x / 2;

  // Calculate Title Text Position
  ImVec1 titleStartPos =
      CalculateTextStart(windowCenterWidth, m_CurrentPlayingTag.title);
  ImGui::SetCursorPos(ImVec2(titleStartPos.x, 330));
  ImGui::Text(m_CurrentPlayingTag.title.c_str());

  // Calculate Artist Text Position
  ImVec1 artistStartPos =
      CalculateTextStart(windowCenterWidth, m_CurrentPlayingTag.artist);
  ImGui::SetCursorPos(ImVec2(artistStartPos.x, 355));
  ImGui::Text(m_CurrentPlayingTag.artist.c_str());

  // Calculate Play Button Position
  ImVec2 playButtonSize(100, 40);
  ImVec2 disconnectButtonSize(100, 40);
  ImVec1 playButtonStart =
      CalculateButtonStart(windowCenterWidth, playButtonSize.x);
  ImGui::SetCursorPos(ImVec2(playButtonStart.x, 380));
  if (ImGui::Button("Play", playButtonSize)) {
  }
  ImVec1 disconnectButtonStart =
      CalculateButtonStart(windowCenterWidth, disconnectButtonSize.x);
  ImGui::SetCursorPos(ImVec2(disconnectButtonStart.x, 440));
  if (ImGui::Button("Disconnect", disconnectButtonSize)) {
    m_Client->Disconnect();
  }
  ImGui::End();
}

bool ClientLayer::isTCPConnected() {
  if (m_Client->GetConnectionStatus() ==
      Walnut::Client::ConnectionStatus::Connected) {
    return true;
  }
  return false;
}
void ClientLayer::DataReceivedCallbackUDP(const std::string& msg) {
  LogMessageCallback("Received Str: " + msg);
}
