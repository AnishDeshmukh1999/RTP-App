#include "ServerLayer.h"

void ServerLayer::OnAttach() {
  m_Console.AddMessage("This is the console!");

  const int port = 1892;
  const std::string address = "127.0.0.1";

  // Song stuff
  m_MP3FileParser = std::make_unique<FileParser::MP3FileParser>(
      "D:\\Programming\\RTP-App\\MP3-Lib\\media\\song2.mp3");
  m_SongInfo = m_MP3FileParser->getInfo();
  MP3::Song song = m_MP3FileParser->getSongDetails();
  m_SongInfo->set_durationseconds(song.m_DurationSeconds);
  m_SongInfo->set_numframes(song.m_numFrames);

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

  // m_MP3ToADUProcessorRunning = true;
  // m_MP3ToADUProcessor = std::thread([this]() { MP3ToADUProcess(); });
  MP3ToADUProcess();
}

void ServerLayer::OnDetach() {
  m_Server->Stop();
  m_MP3ToADUProcessorRunning = false;

  if (m_MP3ToADUProcessor.joinable()) m_MP3ToADUProcessor.join();
}

void ServerLayer::OnUpdate(float ts) {}

void ServerLayer::OnUIRender() { m_Console.OnUIRender(); }

void ServerLayer::LogMessageCallback(const std::string& msg) {
  m_Console.AddMessage(msg);
}

void ServerLayer::MP3ToADUProcess() {
  std::cout << "In the mp3 to ADU process\n";
  ADU::QueueInfo<Segment::SegmentEl> incomingQInfo;
  incomingQInfo.numFrames = m_SongInfo->numframes();
  ADU::QueueInfo<ADUFrameEl> aduFrames =
      ADU::ADU::MP3ToADU(m_MP3FileParser->m_songFrames, incomingQInfo);

  ADU::QueueInfo<Segment::SegmentEl> outgoingQInfo;
  std::vector<std::string> resMP3Frames =
      ADU::ADU::ADUToMP3(outgoingQInfo, aduFrames);
}

void ServerLayer::OnClientConnected(const Walnut::ClientInfo& clientInfo) {
  // Client connection is handled in the PacketType::ClientConnectionRequest
  // case
  m_ClientsConnected.insert(clientInfo.ID);
  m_Console.AddMessage("Client {} connected", clientInfo.ID);
}

void ServerLayer::OnClientDisconnected(const Walnut::ClientInfo& clientInfo) {
  if (m_ClientsConnected.contains(clientInfo.ID)) {
    m_Console.AddMessage("Client {} disconnected", clientInfo.ID);
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
  std::string requestAsString;
  bool success = streamReader.ReadString(requestAsString);
  request.ParseFromString(requestAsString);
  WL_CORE_VERIFY(success);
  if (!success) return;

  Message::Response response;
  LogMessageCallback("Received Data from Client");
  // Check message type and respond appropriately
  if (request.type() == Message::Request::GET_DETAILS) {
    LogMessageCallback("Received Request for Tag");
    response.set_allocated_songinfo(m_SongInfo.get());
    m_MP3FileParser->getSongDetails();
  } else if (request.type() == Message::Request::START_STREAMING) {
    LogMessageCallback("Received Request to Start Streaming");
    // Start sending UDP Packets;
    response.set_response(Message::Response::WILL_START_STREAMING);
  } else if (request.type() == Message::Request::STOP_STREAMING) {
    LogMessageCallback("Received Request to Stop Streaming");
    // Stop sending UDP Packets;
    response.set_response(Message::Response::WILL_STOP_STREAMING);
  }

  Walnut::Buffer responseBuffer;
  responseBuffer.Allocate(response.ByteSizeLong() + sizeof(size_t));
  Walnut::BufferStreamWriter streamWriter(responseBuffer);
  streamWriter.WriteString(response.SerializeAsString());
  m_Server->SendBufferToClient(clientInfo.ID, streamWriter.GetBuffer());
}
