#include "ClientLayer.h"

void ClientLayer::OnAttach() {
  m_Console.AddMessage("This is the Client console!");
}

void ClientLayer::OnDetach() {}

void ClientLayer::OnUpdate(float ts) {}

void ClientLayer::OnUIRender() { m_Console.OnUIRender(); }
