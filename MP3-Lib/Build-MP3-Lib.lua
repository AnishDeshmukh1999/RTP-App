project "MP3-Lib"
   kind "staticlib"
   language "C++"
   cppdialect "C++20"
   targetdir "bin/%{cfg.buildcfg}"
   staticruntime "off"

   files { "Source/**.h", "Source/**.cpp", "Source/**.cc" }

   taglib_include_dir = "../vendor/taglib"
   taglib_include_dir2 = "../vendor/taglib/taglib"
   taglib_lib_dir = "..vendor/taglib/taglib/Debug"
   taglib_toolkit_dir = "../vendor/taglib/taglib/toolkit"
   taglib_id3v2_dir = "../vendor/taglib/taglib/mpeg/id3v2"
   boost_include_dir = "../vendor/boost_1_84_0"
   boost_lib_dir = "../vendor/boost_1_84_0/stage/lib"

   BOOST_ROOT = "../vendor/boost_1_84_0"
   includedirs
   {
      "Source",
      boost_include_dir,
      boost_lib_dir,
      taglib_include_dir,
      taglib_lib_dir,
      taglib_toolkit_dir,
      taglib_include_dir2,
      taglib_id3v2_dir,

      -- Networking Lib
      "../Networking-Lib/Source",

   }

   targetdir ("../bin/" .. outputdir .. "/%{prj.name}")
   objdir ("../bin-int/" .. outputdir .. "/%{prj.name}")

   filter "system:windows"
      systemversion "latest"
      defines { "WL_PLATFORM_WINDOWS" }

   filter "configurations:Debug"
      libdirs {
            path.join(BOOST_ROOT, "stage", "lib"),
            taglib_lib_dir
      }
      links {
            "libboost_filesystem-vc143-mt-gd-x64-1_84",
            "tag"
      }
      defines { "WL_DEBUG" }
      runtime "Debug"
      symbols "On"

   filter "configurations:Release"
      libdirs {
            path.join(BOOST_ROOT, "stage", "lib"),
            taglib_lib_dir
      }
      links {
            "libboost_filesystem-vc143-mt-gd-x64-1_84",
            "tag"
      }
      defines { "WL_RELEASE" }
      runtime "Release"
      optimize "On"
      symbols "On"

   filter "configurations:Dist"
      libdirs {
            path.join(BOOST_ROOT, "stage", "lib"),
            taglib_lib_dir
      }
      links {
            "libboost_filesystem-vc143-mt-gd-x64-1_84",
            "tag"
      }
      kind "WindowedApp"
      defines { "WL_DIST" }
      runtime "Release"
      optimize "On"
      symbols "Off"