-- premake5.lua
workspace "RTP-App"
   architecture "x64"
   configurations { "Debug", "Release", "Dist" }
   startproject "Server-App"

   filter "system:windows"
      buildoptions { "/EHsc", "/Zc:preprocessor", "/Zc:__cplusplus" }

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"
include "Walnut/WalnutExternal.lua"

group "App"
   include "Client-App/Build-Client-App.lua"
   include "Server-App/Build-Server-App.lua"
   include "Networking-Lib/Build-Networking-Lib.lua"
group ""