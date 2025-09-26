# Chip8
---
This repository contains the code for a [Chip-8](https://en.wikipedia.org/wiki/CHIP-8) emulator built in C++20. All code is fully documented. If any instruction or behavior is unclear, you can find additional [Chip-8 information online](https://en.wikipedia.org/wiki/CHIP-8) or follow this [guide](https://tobiasvl.github.io/blog/write-a-chip-8-emulator/#dxyn-display) to build you own emulator.

> [!CAUTION]
> This emulator uses the original [Chip-8](https://en.wikipedia.org/wiki/CHIP-8) stack limit, so only 16 addresses to be pushed!

# Build System
This project uses [Premake5](https://github.com/premake/premake-core) as its build system. A [Premake5](https://github.com/premake/premake-core) instance is included in this repository under Build/[Premake5](https://github.com/premake/premake-core).

The build files are located in the `Build` folder :

| File 						  	 | Usage 				               |
| ------------------------------ | ----------------------------------- |
| `Build/Build.lua` 			 | Define global solution. 	           |
| `Build/Build-Dependencies.lua` | Define dependencies solution.  	   |
| `Build/Build-Chip8.lua` 		 | Define chip8 library solution. 	   |
| `Build/Build-Example.lua` 	 | Define example executable solution. |

## Windows
To build on Windows, you need at least `Visual Studio 2022 Community Edition` or another `Visual Studio C++` installation with `C++20` support.

Run the following to generate the project files :

'''bat
:: Windows batch file
Setup-Windows.bat
'''

## Linux
> [!CAUTION]
> Linux support has not been tested yet.

To build on Linux, you need a valid `GCC` installation with `C++20` support.

Run the following to generate the project files :

'''sh
# Linux shell script
Setup-Linux.sh
'''
