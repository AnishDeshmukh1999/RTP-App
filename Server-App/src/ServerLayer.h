#pragma once

#include "HeadlessConsole.h"
#include "Networking/Server/Server.h"
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
  std::unique_ptr<Networking::Server> m_Server{nullptr};
  void LogMessageCallback(const std::string& msg);
};
