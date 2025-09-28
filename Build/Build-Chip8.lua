project "chip8"
	kind "StaticLib"
	language "C++"
	staticruntime "off"
	location "%{OutputDirs.Solution}"

	--- OUTPUT
	targetdir "%{OutputDirs.Bin}%{cfg.buildcfg}/"
	debugdir "%{OutputDirs.Bin}%{cfg.buildcfg}/"
	objdir "%{OutputDirs.BinInt}%{prj.name}-%{cfg.buildcfg}"

	--- SOURCE FILES
	files {
		"%{IncludeDirs.chip8}**.h",
		"%{IncludeDirs.chip8}chip8.cpp",
		"%{IncludeDirs.chip8}chip8_cmu.cpp",
		"%{IncludeDirs.chip8}chip8_cpu_implementation.cpp",
		"%{IncludeDirs.chip8}chip8_cpu_opcode_manager.cpp",
		"%{IncludeDirs.chip8}chip8_cpu_option_manager.cpp",
		"%{IncludeDirs.chip8}chip8_cpu_timer_manager.cpp",
		"%{IncludeDirs.chip8}chip8_mmu.cpp",
		"%{IncludeDirs.chip8}chip8_rmu.cpp",
		"%{IncludeDirs.chip8}chip8_smu.cpp"
	}

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
