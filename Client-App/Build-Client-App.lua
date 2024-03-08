project "Client-App"
   kind "ConsoleApp"
   language "C++"
   cppdialect "C++20"
   targetdir "bin/%{cfg.buildcfg}"
   staticruntime "off"

   files { "src/**.h", "src/**.cpp" }

   boost_include_dir = "../vendor/boost_1_84_0"
   taglib_include_dir = "../vendor/taglib"
   taglib_include_dir2 = "../vendor/taglib/taglib"
   taglib_lib_dir = "..vendor/taglib/taglib/Debug"
   taglib_toolkit_dir = "../vendor/taglib/taglib/toolkit"
   taglib_id3v2_dir = "../vendor/taglib/taglib/mpeg/id3v2"
   includedirs
   {

      "../Walnut/vendor/imgui",
      "../Walnut/vendor/glfw/include",
      "../Walnut/vendor/glm",
      "../Walnut/vendor/spdlog/include",

      "../Walnut/Walnut/Platform/Headless",
      "../Walnut/Walnut/Platform/GUI",
      "../Walnut/Walnut/Source/",

      "%{IncludeDir.VulkanSDK}",

      --taglib
      taglib_include_dir,
      taglib_lib_dir,
      taglib_toolkit_dir,
      taglib_include_dir2,
      taglib_id3v2_dir,

      -- Networking Lib
      "../Networking-Lib/Source",

      -- MP3 Lib
      "../MP3-Lib/Source",

      -- Walnut-Networking
      "../Walnut/Walnut-Modules/Walnut-Networking/Source",
      "../Walnut/Walnut-Modules/Walnut-Networking/vendor/GameNetworkingSockets/include",
      
      boost_include_dir
   }

   links
   {
       "Walnut",
       "Networking-Lib",
       "MP3-Lib"
   }

   targetdir ("../bin/" .. outputdir .. "/%{prj.name}")
   objdir ("../bin-int/" .. outputdir .. "/%{prj.name}")

   filter "system:windows"
      systemversion "latest"
      defines { "WL_PLATFORM_WINDOWS" }
      postbuildcommands 
      {
            '{COPY} "../%{WalnutNetworkingBinDir}/GameNetworkingSockets.dll" "%{cfg.targetdir}"',
            '{COPY} "../%{WalnutNetworkingBinDir}/libcrypto-3-x64.dll" "%{cfg.targetdir}"',
            '{COPY} "../%{WalnutNetworkingBinDir}/libprotobufd.dll" "%{cfg.targetdir}"',
      }

   filter "configurations:Debug"
      libdirs {
         taglib_lib_dir
      }
      links {
         "tag"
      }
      defines { "WL_DEBUG" }
      runtime "Debug"
      symbols "On"

   filter "configurations:Release"
      libdirs {
         taglib_lib_dir
      }
      links {
         "tag"
      }
      defines { "WL_RELEASE" }
      runtime "Release"
      optimize "On"
      symbols "On"

   filter "configurations:Dist"
      libdirs {
         taglib_lib_dir
      }
      links {
         "tag"
      }
      kind "WindowedApp"
      defines { "WL_DIST" }
      runtime "Release"
      optimize "On"
      symbols "Off"