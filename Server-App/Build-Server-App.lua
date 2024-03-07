project "Server-App"
   kind "ConsoleApp"
   language "C++"
   cppdialect "C++20"
   targetdir "bin/%{cfg.buildcfg}"
   staticruntime "off"

   files { "src/**.h", "src/**.cpp" }

   boost_include_dir = "../vendor/boost_1_84_0"
   boost_lib_dir = "../vendor/boost_1_84_0/stage/lib"
   BOOST_ROOT = "../vendor/boost_1_84_0"
   includedirs
   {

      "../Walnut/vendor/imgui",
      "../Walnut/vendor/glfw/include",
      "../Walnut/vendor/glm",

      "../Walnut/Walnut/Platform/Headless",
      "../Walnut/Walnut/Platform/GUI",
      "../Walnut/Walnut/Source/",

      "../vendor/spdlog/include",

      "%{IncludeDir.VulkanSDK}",

      -- Networking Lib
      "../Networking-Lib/Source",

      -- MP3 Lib
      "../MP3-Lib/Source",

      boost_include_dir,
      boost_lib_dir
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

   filter "configurations:Debug"
   libdirs {
      path.join(BOOST_ROOT, "stage", "lib"),
}
links {
      "libboost_filesystem-vc143-mt-gd-x64-1_84",
}
      defines { "WL_DEBUG" }
      runtime "Debug"
      symbols "On"

   filter "configurations:Release"
   libdirs {
      path.join(BOOST_ROOT, "stage", "lib"),
}
links {
      "libboost_filesystem-vc143-mt-gd-x64-1_84",
}
      defines { "WL_RELEASE" }
      runtime "Release"
      optimize "On"
      symbols "On"

   filter "configurations:Dist"
   libdirs {
      path.join(BOOST_ROOT, "stage", "lib"),
}
links {
      "libboost_filesystem-vc143-mt-gd-x64-1_84",
}
      kind "WindowedApp"
      defines { "WL_DIST" }
      runtime "Release"
      optimize "On"
      symbols "Off"