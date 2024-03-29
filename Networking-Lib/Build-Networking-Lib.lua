project "Networking-Lib"
   kind "staticlib"
   language "C++"
   cppdialect "C++20"
   targetdir "bin/%{cfg.buildcfg}"
   staticruntime "off"

   files { "Source/**.h", "Source/**.cpp", "Source/**.cc" }

   boost_lib_dir = "../vendor/boost_1_84_0/stage/lib"
   boost_include_dir = "../vendor/boost_1_84_0"
   taglib_include_dir = "../vendor/taglib"
   taglib_lib_dir = "..vendor/taglib/taglib/Debug"

   includedirs
   {
      "Source",

      "../Walnut/vendor/imgui",
      "../Walnut/vendor/glfw/include",
      "../Walnut/vendor/glm",
      "../Walnut/vendor/spdlog/include",

      "../Walnut/Walnut/Platform/Headless",
      "../Walnut/Walnut/Platform/GUI",
      "../Walnut/Walnut/Source/",

      "../Walnut/Walnut/src",

      boost_include_dir,
      taglib_include_dir,
      taglib_lib_dir
   }

   links{
      "Walnut-Networking"
   }

   targetdir ("../bin/" .. outputdir .. "/%{prj.name}")
   objdir ("../bin-int/" .. outputdir .. "/%{prj.name}")

   filter "system:windows"
      systemversion "latest"
      defines { "WL_PLATFORM_WINDOWS" }

   filter "configurations:Debug"
      defines { "WL_DEBUG" }
      runtime "Debug"
      symbols "On"

   filter "configurations:Release"
      defines { "WL_RELEASE" }
      runtime "Release"
      optimize "On"
      symbols "On"

   filter "configurations:Dist"
      kind "WindowedApp"
      defines { "WL_DIST" }
      runtime "Release"
      optimize "On"
      symbols "Off"