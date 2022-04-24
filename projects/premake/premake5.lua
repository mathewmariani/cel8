workspace "cel8"
  configurations { "Debug", "Release" }
  platforms { "64bit", "32bit" }
  defaultplatform "32bit"
  startproject "cel8"
  location ("../" .. _ACTION)

  filter "configurations:Debug"
    targetsuffix "_d"
    defines { "DEBUG" }
    symbols "On"

  filter "configurations:Release"
    defines { "nDEBUG" }
    optimize "On"

  filter { "action:vs2017" }
    location ("../vs2017")
    
  filter { "action:xcode4" }
    location ("../xcode4")

  filter { "action:gmake2" }
    location ("../make")

  filter { "action:gmake2", "system:macosx" }
    location ("../make.mac")

  filter { "platforms:32bit" }
    architecture "x86"

  filter { "platforms:64bit" }
    architecture "x86_64"

  filter { "system:windows" }
    systemversion "latest"
    defines { "_CRT_SECURE_NO_WARNINGS" }

  filter { "system:linux" }
    links { "m" }

project "cel8"
  kind "ConsoleApp"
  language "C"
  cdialect "C99"
  targetdir "../../build"
  basedir "../../build"
  debugdir "../../build"

  -- cel8:
  files {
    "../../src/cel8.h",
    "../../src/lib/cel8-sokol.c",
  }

  -- embed:
  files {
    "../../src/embed/**.h"
  }

  -- lua:
  files {
    "../../src/lib/lua/**.c",
    "../../src/lib/lua/**.h",
  }

  -- sokol:
  filter "system:windows"
    files {
      "../../src/lib/sokol/sokol.c",
      "../../src/lib/sokol/**.h",
    }

  -- sokol:
  filter "system:macosx"
    files {
      "../../src/lib/sokol/sokol.m",
      "../../src/lib/sokol/**.h",
    }

  includedirs {
    "../../src",
  }

  filter "system:windows"
    links {
      "OpenGL32",
    }

  filter "system:macosx"
    links { 
      "Cocoa.framework",
      "QuartzCore.framework",
      "OpenGL.framework",
      "Foundation.framework",
    }

