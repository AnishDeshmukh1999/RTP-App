#pragma warning(push, 0)
// Some includes with unfixable warnings
#include "ServerLayer.h"
#include "Walnut/Application.h"
#include "Walnut/EntryPoint.h"
#pragma warning(pop)

Walnut::Application* Walnut::CreateApplication(int argc, char** argv) {
  Walnut::ApplicationSpecification spec;
  spec.Name = "RTP Server Application ";
  spec.Height = 500;
  spec.Width = 800;

  Walnut::Application* application = new Walnut::Application(spec);

  application->PushLayer<ServerLayer>();

#ifndef SERVER_HEADLESS

  application->SetMenubarCallback([application]() {
    ImGui::ShowDemoWindow();
    if (ImGui::BeginMenu("File")) {
      if (ImGui::MenuItem("Exit")) {
        application->Close();
      }
      ImGui::EndMenu();
    }
  });
#endif  // !SERVER_HEADLESS
  return application;
}