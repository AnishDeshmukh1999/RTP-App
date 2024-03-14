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

  m_MP3ToADUProcessorRunning = true;
  m_MP3ToADUProcessor = std::thread([this]() { MP3ToADUProcess(); });
  m_RTPSenderRunning = true;
  m_RTPSender = std::thread([this]() { RTPSender(); });
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
  LogMessageCallback("converting MP3 frames to ADU! \n");
  ADU::QueueInfo<Segment::SegmentEl> incomingQInfo;
  ADU::QueueInfo<ADUFrameEl> aduFrames =
      ADU::ADU::MP3ToADU(m_MP3FileParser->m_songFrames, incomingQInfo);
  std::lock_guard<std::mutex> lock(m_mtx);
  std::vector<std::string> aduFramesVec;
  ADU::ADU::interleaveFrames(aduFrames, aduFramesVec);
  size_t maxSize = 0;
  for (auto el : m_framesToSend) {
    maxSize = std::max(maxSize, el.length());
  }
  // Implement ADU Descriptor handling for packets with size >= MTU
  for (auto t_frame : aduFramesVec) {
    std::string aduWithDescriptor = ADU::ADU::addAduDescriptor(t_frame);
  }
  LogMessageCallback("Done converting frames! \n");
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
    std::lock_guard lk(m_Sendingmutex);
    m_StartSendingRTP = true;
    std::cout << "m_StartSendingRTP: " << (m_StartSendingRTP ? "true" : "false")
              << '\n';
    m_cv.notify_one();
    response.set_response(Message::Response::WILL_START_STREAMING);
  } else if (request.type() == Message::Request::STOP_STREAMING) {
    LogMessageCallback("Received Request to Stop Streaming");
    std::lock_guard lk(m_Sendingmutex);
    m_StartSendingRTP = false;
    std::cout << "m_StartSendingRTP: " << (m_StartSendingRTP ? "true" : "false")
              << '\n';
    m_cv.notify_one();
    // Stop sending UDP Packets;
    response.set_response(Message::Response::WILL_STOP_STREAMING);
  }

  Walnut::Buffer responseBuffer;
  responseBuffer.Allocate(response.ByteSizeLong() + sizeof(size_t));
  Walnut::BufferStreamWriter streamWriter(responseBuffer);
  streamWriter.WriteString(response.SerializeAsString());
  m_Server->SendBufferToClient(clientInfo.ID, streamWriter.GetBuffer());
}

void ServerLayer::RTPSender() {
  while (m_RTPSenderRunning) {
    {
      std::unique_lock<std::mutex> lk(m_Sendingmutex);
      m_cv.wait(lk, [this] { return m_StartSendingRTP; });
    }
    std::string msg =
        "Sending RTP! idx: " + std::to_string(m_currentSendPacketIndex);
    LogMessageCallback(msg);
    std::this_thread::sleep_for(std::chrono::milliseconds(50));
  }
}
