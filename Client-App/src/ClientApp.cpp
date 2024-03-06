#include "ClientLayer.h"
#include "Walnut/Application.h"
#include "Walnut/EntryPoint.h"

Walnut::Application* Walnut::CreateApplication(int argc, char** argv) {
  Walnut::ApplicationSpecification spec;
  spec.Name = "RTP Client Application ";

  std::shared_ptr<ClientLayer> clientLayer = std::make_shared<ClientLayer>();

  Walnut::Application* application = new Walnut::Application(spec);
  application->PushLayer(clientLayer);
  application->SetMenubarCallback([application, clientLayer]() {
    // ImGui::ShowDemoWindow();
    if (ImGui::BeginMenu("File")) {
      if (ImGui::MenuItem("Exit")) {
        application->Close();
      }
      ImGui::EndMenu();
    }
  });

  return application;
}
