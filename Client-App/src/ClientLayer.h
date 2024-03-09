#include <string>

#include "MP3/MP3.h"
#include "Networking/ClientUDP/ClientUDP.h"
#include "Networking/Message.pb.h"
#include "Networking/Util/Util.h"
#include "Walnut/Image.h"
#include "Walnut/Layer.h"
#include "Walnut/Networking/Client.h"
#include "Walnut/Serialization/BufferStream.h"
#include "Walnut/UI/Console.h"

#pragma once
class ClientLayer : public Walnut::Layer {
 public:
  virtual void OnAttach() override;
  virtual void OnDetach() override;
  virtual void OnUIRender() override;

  // Server event callbacks
  void OnConnected();
  void OnDisconnected();
  void OnDataReceived(const Walnut::Buffer buffer);

 private:
  Walnut::UI::Console m_Console{"Client Console"};
  std::unique_ptr<Networking::ClientUDP> m_ClientUDP;
  std::unique_ptr<Walnut::Client> m_Client;
  int m_port = 1982;
  std::string m_address;
  bool m_ConnectionModalOpen = false;
  MP3::Tag m_CurrentPlayingTag;
  Walnut::Buffer m_buffer;
  std::string m_DefaultImagePath =
      "D:\\Programming\\RTP-App\\Client-App\\res\\default-album-art.jpg";
  std::unique_ptr<Walnut::Image> m_AlbumArtPtr;
  std::string m_CurrentPlayingImage;
  char* m_imageDataCharPtr{};
  void UI_ConnectionModal();
  void ConnectedUIRender();
  bool isTCPConnected();
  void LogMessageCallback(const std::string& msg);
  void DataReceivedCallbackUDP(const std::string& msg);
};
