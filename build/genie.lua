solution "PlanetRenderer"
    configurations {
        "Release",
        "Debug"
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
        objdir "obj/debug"
	flags {"OptimizeSpeed", "No64BitChecks"}

    configuration "vs*"
        flags { "Unicode", "Winmain"}
        defines { "WIN32", "PLATFORM_Win" }
		links { "ILUT" }
    configuration { "linux", "gmake"}
--	flags {"-pedantic"}--
	defines { "PLATFORM_Linux", "__linux__" }
	includedirs { "/usr/include" }

    configuration {}

	buildoptions_cpp
	{
		"-std=c++14",
	}

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
		"IL",
		"ILU"
    }
    --pchheader path.join(SOURCE_DIR, "stdafx.h")
    --pchsource path.join(SOURCE_DIR, "stdafx.cpp")

    -- optional. This is purely cosmetically.
    vpaths {
    }
