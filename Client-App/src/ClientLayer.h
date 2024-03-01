#include <string>

#include "Networking/Client/Client.h"
#include "Networking/Server/Server.h"
#include "Walnut/Layer.h"
#include "Walnut/UI/Console.h"

#pragma once
class ClientLayer : public Walnut::Layer {
 public:
  virtual void OnAttach() override;
  virtual void OnDetach() override;
  virtual void OnUIRender() override;

 private:
  Walnut::UI::Console m_Console{"Client Console"};
  std::unique_ptr<Networking::Client> m_Client;
  void LogMessageCallback(const std::string& msg);
  void DataReceivedCallback(const std::string& data);
};
