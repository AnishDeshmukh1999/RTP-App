#pragma once

#define _SILENCE_ALL_MS_EXT_DEPRECATION_WARNINGS
#define _CRT_SECURE_NO_WARNINGS

#include "HeadlessConsole.h"
#include "MP3/MP3FileParser.h"
#include "Networking/ServerUDP/ServerUDP.h"
#include "Walnut/Layer.h"
#include "Walnut/UI/Console.h"

class ServerLayer : public Walnut::Layer {
 public:
  virtual void OnAttach() override;
  virtual void OnDetach() override;
  virtual void OnUpdate(float ts) override;
  virtual void OnUIRender() override;

 private:
#ifdef SERVER_HEADLESS
  HeadlessConsole m_console{"Server Console"};
#else
  Walnut::UI::Console m_Console{"Server Console"};
#endif  //
  std::unique_ptr<Networking::ServerUDP> m_Server{nullptr};
  std::unique_ptr<FileParser::MP3FileParser> m_MP3FileParser{nullptr};
  std::unique_ptr<ID3v2::ID3v2Tag> m_SongID3v2Tag{nullptr};
  void LogMessageCallback(const std::string& msg);
};
