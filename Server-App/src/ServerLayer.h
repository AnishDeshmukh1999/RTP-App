#pragma once

#define _SILENCE_ALL_MS_EXT_DEPRECATION_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
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

  std::unique_ptr<FileParser::MP3FileParser> m_MP3FileParser{nullptr};
  std::unique_ptr<Message::SongInfo> m_SongInfo{nullptr};
  std::set<Walnut::ClientID> m_ClientsConnected;
  void LogMessageCallback(const std::string& msg);
};
