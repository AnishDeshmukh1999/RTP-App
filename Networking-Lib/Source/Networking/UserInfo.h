#pragma once
#include "Walnut/Serialization/StreamReader.h"
#include "Walnut/Serialization/StreamWriter.h"

struct UserInfo {
  uint32_t Color;
  std::string Username;

  static void Serialize(Walnut::StreamWriter* serializer,
                        const UserInfo& instance) {
    serializer->WriteRaw(instance.Color);
    serializer->WriteString(instance.Username);
  }

  static void Deserialize(Walnut::StreamReader* deserializer,
                          UserInfo& instance) {
    deserializer->ReadRaw(instance.Color);
    deserializer->ReadString(instance.Username);
  }
};
