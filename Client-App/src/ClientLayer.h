#include <string>

#include "Walnut/Layer.h"
#include "Walnut/UI/Console.h"

#pragma once
class ClientLayer : public Walnut::Layer {
 public:
  virtual void OnAttach() override;
  virtual void OnDetach() override;
  virtual void OnUpdate(float ts) override;
  virtual void OnUIRender() override;

 private:
  std::string m_serverIP;
  Walnut::UI::Console m_Console{"Client Console"};
};
