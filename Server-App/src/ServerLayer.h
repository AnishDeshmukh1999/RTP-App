#pragma once

#define _SILENCE_ALL_MS_EXT_DEPRECATION_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#include <MP3/ADU/ADU.h>
#include <MP3/Segment.h>

#include <mutex>

#include "HeadlessConsole.h"
#include "MP3/MP3.h"
#include "MP3/MP3FileParser.h"
#include "Networking/Message.pb.h"
#include "Networking/ServerUDP/ServerUDP.h"
#include "Networking/UserInfo.h"
#include "Walnut/Layer.h"
#include "Walnut/Networking/Server.h"
#include "Walnut/Serialization/BufferStream.h"
#include "Walnut/UI/Console.h"

class ServerLayer : public Walnut::Layer {
 public:
  virtual void OnAttach() override;
  virtual void OnDetach() override;
  virtual void OnUpdate(float ts) override;
  virtual void OnUIRender() override;

  // Server event callbacks
  void OnClientConnected(const Walnut::ClientInfo& clientInfo);
  void OnClientDisconnected(const Walnut::ClientInfo& clientInfo);
  void OnDataReceived(const Walnut::ClientInfo& clientInfo,
                      const Walnut::Buffer buffer);

 private:
#ifdef SERVER_HEADLESS
  HeadlessConsole m_console{"Server Console"};
#else
  Walnut::UI::Console m_Console{"Server Console"};
#endif  //
  std::unique_ptr<Networking::ServerUDP> m_ServerUDP{nullptr};
  std::unique_ptr<Walnut::Server> m_Server{nullptr};
  std::thread m_MP3ToADUProcessor;
  bool m_MP3ToADUProcessorRunning{false};
  std::unique_ptr<FileParser::MP3FileParser> m_MP3FileParser{nullptr};
  std::unique_ptr<Message::SongInfo> m_SongInfo{nullptr};
  std::mutex m_mtx;
  std::vector<std::string> m_framesToSend{};
  std::set<Walnut::ClientID> m_ClientsConnected;
  int m_currentSendPacketIndex{};
  std::thread m_RTPSender;
  bool m_RTPSenderRunning{false};
  bool m_StartSendingRTP{false};
  std::condition_variable m_cv;
  std::mutex m_Sendingmutex;
  void RTPSender();
  void LogMessageCallback(const std::string& msg);
  void MP3ToADUProcess();
};
