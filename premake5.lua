-- premake5.lua
workspace "RTP-App"
   architecture "x64"
   configurations { "Debug", "Release", "Dist" }
   startproject "Server-App"

   filter "system:windows"
      buildoptions { "/EHsc", "/Zc:preprocessor", "/Zc:__cplusplus" }


outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"
WalnutNetworkingBinDir = "Walnut/Walnut-Modules/Walnut-Networking/vendor/GameNetworkingSockets/bin/%{cfg.system}/%{cfg.buildcfg}/"

include "Walnut/Build-Walnut-External.lua"

group "App"
   include "Client-App/Build-Client-App.lua"
   include "Server-App/Build-Server-App.lua"
   include "Networking-Lib/Build-Networking-Lib.lua"
   include "MP3-Lib/Build-MP3-Lib.lua"
group ""