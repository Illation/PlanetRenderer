solution "PlanetRenderer"
    configurations {
        "Release",
        "Debug",
		"Shipping"
    }

    platforms {
        "x64",
        "x32",
        "Native" -- for targets where bitness is not specified
    }

    language "C++"

    location "../"
    objdir "../build/"

PROJECT_DIR = "../"
SOURCE_DIR = path.join(PROJECT_DIR, "PlanetFramework/")


project "PlanetFramework"
    kind "ConsoleApp"

    configuration "Debug"
        targetdir "../bin/debug/"
        objdir "obj/debug"
        defines { "_DEBUG" }
        flags { "Symbols" }
    configuration "Release"
        targetdir "../bin/release/"
        objdir "obj/release"
		flags {"OptimizeSpeed", "Symbols" }
    configuration "Shipping"
        targetdir "../bin/shipping/"
        objdir "obj/shipping"
		flags {"OptimizeSpeed", "No64BitChecks" }

    configuration "vs*"
        flags { "Unicode", "Winmain"}
        defines { "WIN32", "PLATFORM_Win" }
		includedirs { "$(S_LIBS)/SDL2/include", "$(S_LIBS)/DevIL/include", "$(S_LIBS)/glm" }
		debugdir "$(OutDir)"
		libdirs { "$(S_LIBS)/SDL2/lib/x86", "$(S_LIBS)/DevIL/lib/x86/unicode/Release" }
		links { "DevIL", "ILUT" }
		postbuildcommands { 
			"xcopy \"$(S_LIBS)\\SDL2\\lib\\x86\\SDL2.dll\" \"$(OutDir)\" /y/D",
			"xcopy \"$(S_LIBS)\\DevIL\\lib\\x86\\unicode\\Release\\ILU.dll\" \"$(OutDir)\" /y/D",
			"xcopy \"$(S_LIBS)\\DevIL\\lib\\x86\\unicode\\Release\\ILUT.dll\" \"$(OutDir)\" /y/D",
			"xcopy \"$(S_LIBS)\\DevIL\\lib\\x86\\unicode\\Release\\DevIL.dll\" \"$(OutDir)\" /y/D",
			
			"xcopy \"$(SolutionDir)PlanetFramework\\Fonts\" \"$(OutDir)\\Fonts\" /s/i/y",
			"xcopy \"$(SolutionDir)PlanetFramework\\Textures\" \"$(OutDir)\\Textures\" /s/i/y",
			"xcopy \"$(SolutionDir)PlanetFramework\\Shaders\" \"$(OutDir)\\Shaders\" /s/i/y" 
		}
    configuration { "linux", "gmake"}
	--	flags {"-pedantic"}--
		defines { "PLATFORM_Linux", "__linux__" }
		includedirs { "/usr/include" }
		buildoptions_cpp { "-std=c++14" }
		links { "IL" }
    configuration {}

    defines { "_CONSOLE" }

    files {
        path.join(SOURCE_DIR, "*.cpp"),
        path.join(SOURCE_DIR, "*.h"),
        path.join(path.join(SOURCE_DIR, "PlanetTech/"), "*.cpp"),
        path.join(path.join(SOURCE_DIR, "PlanetTech/"), "*.h"),
        path.join(path.join(SOURCE_DIR, "PlanetTech/Types/"), "*.cpp"),
        path.join(path.join(SOURCE_DIR, "PlanetTech/Types/"), "*.h"),
        path.join(path.join(SOURCE_DIR, "StaticDependancies/glad/"), "*.cpp"),
        path.join(path.join(SOURCE_DIR, "StaticDependancies/glad/"), "*.h"),
    }

    excludes {
		path.join(SOURCE_DIR, "Screenshot.cpp"),
		path.join(SOURCE_DIR, "Screenshot.h"),
    }
    links{
    	"SDL2",
		"ILU"
    }
    --pchheader path.join(SOURCE_DIR, "stdafx.h")
    --pchsource path.join(SOURCE_DIR, "stdafx.cpp")

    -- optional. This is purely cosmetical.
    vpaths {
    }
