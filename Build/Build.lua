--- TOOL INCLUDES
include 'Premake/CSExtensions.lua'
include 'Premake/VSExtensions.lua'
include 'Build-Dependencies.lua'

--- PROJECT CONFIGURATION
workspace 'Chip8'
    startproject 'chip8_example'
    defaultplatform 'x64'
    location '../'
    configurations { 
        'Debug', 
        'Release', 
        'Dist' 
    }

    --- WINDOWS
    filter 'system:windows'
        --- GLOBAL FLAGS
        flags 'MultiProcessorCompile'
        
        --- WORKSPACE-WIDE BUILD OPTIONS FOR MSVC
        buildoptions { 
            '/EHsc',
            '/Zc:preprocessor', 
            '/Zc:__cplusplus' 
        }

    --- PROJECTS
    include 'Build-Chip8.lua'
    include 'Build-Dap.lua'
    include 'Build-Example.lua'
