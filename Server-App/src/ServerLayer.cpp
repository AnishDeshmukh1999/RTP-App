#include "ServerLayer.h"

void ServerLayer::OnAttach() {
  m_Console.AddMessage("This is the console!");

  const int port = 1982;
  const std::string address = "127.0.0.1";

  // Allocate and start TCP Server
  m_Server = std::make_unique<Walnut::Server>(port);
  m_Server->SetClientConnectedCallback(
      [this](const Walnut::ClientInfo& clientInfo) {
        OnClientConnected(clientInfo);
      });
  m_Server->SetClientDisconnectedCallback(
      [this](const Walnut::ClientInfo& clientInfo) {
        OnClientDisconnected(clientInfo);
      });
  m_Server->SetDataReceivedCallback(
      [this](const Walnut::ClientInfo& clientInfo, const Walnut::Buffer data) {
        OnDataReceived(clientInfo, data);
      });
  m_Server->Start();

  m_ServerUDP = std::make_unique<Networking::ServerUDP>(address, port);
  m_ServerUDP->SetLogMessageCallback(
      [this](const std::string& msg) { LogMessageCallback(msg); });
  // m_Server->Start();
}

void ServerLayer::OnDetach() { m_Server->Stop(); }

void ServerLayer::OnUpdate(float ts) {}

void ServerLayer::OnUIRender() { m_Console.OnUIRender(); }

void ServerLayer::LogMessageCallback(const std::string& msg) {
  m_Console.AddMessage(msg);
}

void ServerLayer::OnClientConnected(const Walnut::ClientInfo& clientInfo) {
  // Client connection is handled in the PacketType::ClientConnectionRequest
  // case
}

void ServerLayer::OnClientDisconnected(const Walnut::ClientInfo& clientInfo) {
  if (m_ClientsConnected.contains(clientInfo.ID)) {
    const auto& userInfo = m_ClientsConnected.at(clientInfo.ID);
    m_Console.AddItalicMessage("Client {} disconnected", userInfo.Username);
    m_ClientsConnected.erase(clientInfo.ID);
  } else {
    std::cout << "[ERROR] OnClientDisconnected - Could not find client with ID="
              << clientInfo.ID << std::endl;
    std::cout << "  ConnectionDesc=" << clientInfo.ConnectionDesc << std::endl;
  }
}

void ServerLayer::OnDataReceived(const Walnut::ClientInfo& clientInfo,
                                 const Walnut::Buffer buffer) {
  Walnut::BufferStreamReader streamReader(buffer);
  Message::Request request;
  bool success = streamReader.ReadRaw<Message::Request>(request);
  WL_CORE_VERIFY(success);
  if (!success) return;

  Message::Response response;

  // Check message type and respond appropriately
  if (request.type() & Message::Request::GET_ID3_TAG) {
    m_MP3FileParser = std::make_unique<FileParser::MP3FileParser>(
        "D:\\Programming\\RTP-App\\MP3-Lib\\media\\song-ID3.mp3");
    m_SongID3v2Tag = m_MP3FileParser->getTag();
    response.set_allocated_id3v2tag(m_SongID3v2Tag.get());
  } else if (request.type() & Message::Request::START_STREAMING) {
    response.set_response(Message::Response::SUCCESS);
  } else if (request.type() & Message::Request::STOP_STREAMING) {
    response.set_response(Message::Response::SUCCESS);
  }

  Walnut::Buffer responseBuffer;
  Walnut::BufferStreamWriter streamWriter(responseBuffer);
  streamWriter.WriteRaw<Message::Response>(response);
  m_Server->SendBufferToClient(clientInfo.ID, streamWriter.GetBuffer());
}
