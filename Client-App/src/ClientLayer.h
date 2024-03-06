#include <string>

#include "Networking/ClientTCP/ClientTCP.h"
#include "Networking/ClientUDP/ClientUDP.h"
#include "Networking/Util/Util.h"
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
  std::unique_ptr<Networking::ClientUDP> m_ClientUDP;
  std::unique_ptr<Networking::ClientTCP> m_ClientTCP;
  int m_port = 1982;
  std::string m_address;
  bool m_ConnectionModalOpen = false;
  void UI_ConnectionModal();
  void ConnectedUIRender();
  bool isTCPConnected();
  void LogMessageCallback(const std::string& msg);
  void DataReceivedCallback(const std::string& data);
};
