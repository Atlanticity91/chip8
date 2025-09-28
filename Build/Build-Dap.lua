project "chip8_dap"
	kind "ConsoleApp"
	language "C++"
	staticruntime "off"
	location "%{OutputDirs.Solution}"

	--- OUTPUT
	targetdir "%{OutputDirs.Bin}%{cfg.buildcfg}/"
	debugdir "%{OutputDirs.Bin}%{cfg.buildcfg}/"
	objdir "%{OutputDirs.BinInt}%{prj.name}-%{cfg.buildcfg}"

	--- INCLUDES DIRS
	includedirs "%{IncludeDirs.chip8}"
	externalincludedirs "%{IncludeDirs.chip8}"

	--- SOURCE FILES
	files {
        "%{IncludeDirs.chip8_dap}**.h",	
        "%{IncludeDirs.chip8_dap}**.cpp"
    }

	links "chip8"

	--- LINUX
	filter "system:linux"
		--- LINUX SPECIFIC DEFINES
		defines { "LINUX" }

	--- WINDOWS
	filter "system:windows"
		cppdialect "C++20"
		flags "MultiProcessorCompile"

		--- WINDOWS SPECIFIC DEFINES
		defines { "WINDOWS" }

	--- CONFIGURATION
	filter "configurations:Debug"
		runtime "Debug"
		symbols "On"

		--- DEFINES
		defines { "DEBUG" }

	filter "configurations:Release"
		runtime "Release"
		optimize "On"
		symbols "On"

		--- DEFINES
		defines { "RELEASE" }

	filter "configurations:Dist"
		runtime "Release"
		optimize "On"
		symbols "Off"

		--- DEFINES
		defines { "DIST" }
