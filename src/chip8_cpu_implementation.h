#pragma once

#include "chip8_cmu.h"

struct chip8_cpu_manager_unit;

/**
 * chip8_cpu_implementation namespace
 * @note Store all instruction implementation as exec_instruction_name.
 **/
namespace chip8_cpu_implementation {

    echip8_states exec_0GGG_routine(
        const uint16_t instruction,
        chip8_cpu_manager_unit& cpu,
        chip8_memory_manager_unit& mmu,
        chip8_screen_manager_unit& smu
    );

    echip8_states exec_1NNN_jump( 
        const uint16_t instruction,
        chip8_cpu_manager_unit& cpu,
        chip8_memory_manager_unit& mmu,
        chip8_screen_manager_unit& smu
    );

    echip8_states exec_2NNN_subroutines( 
        const uint16_t instruction,
        chip8_cpu_manager_unit& cpu,
        chip8_memory_manager_unit& mmu,
        chip8_screen_manager_unit& smu
    );

    echip8_states exec_3XNN_skip( 
        const uint16_t instruction,
        chip8_cpu_manager_unit& cpu,
        chip8_memory_manager_unit& mmu,
        chip8_screen_manager_unit& smu
    );

    echip8_states exec_4XNN_skip( 
        const uint16_t instruction,
        chip8_cpu_manager_unit& cpu,
        chip8_memory_manager_unit& mmu,
        chip8_screen_manager_unit& smu
    );
    
    echip8_states exec_5XY0_skip( 
        const uint16_t instruction,
        chip8_cpu_manager_unit& cpu,
        chip8_memory_manager_unit& mmu,
        chip8_screen_manager_unit& smu
    );

    echip8_states exec_6XNN_set( 
        const uint16_t instruction,
        chip8_cpu_manager_unit& cpu,
        chip8_memory_manager_unit& mmu,
        chip8_screen_manager_unit& smu
    );

    echip8_states exec_7XNN_add( 
        const uint16_t instruction,
        chip8_cpu_manager_unit& cpu,
        chip8_memory_manager_unit& mmu,
        chip8_screen_manager_unit& smu
    );

    echip8_states exec_8XYG_logic(
        const uint16_t instruction,
        chip8_cpu_manager_unit& cpu,
        chip8_memory_manager_unit& mmu,
        chip8_screen_manager_unit& smu
    );

    echip8_states exec_9XY0_skip( 
        const uint16_t instruction,
        chip8_cpu_manager_unit& cpu,
        chip8_memory_manager_unit& mmu,
        chip8_screen_manager_unit& smu
    );

    echip8_states exec_ANNN_set_i( 
        const uint16_t instruction,
        chip8_cpu_manager_unit& cpu,
        chip8_memory_manager_unit& mmu,
        chip8_screen_manager_unit& smu
    );
    
    echip8_states exec_BNNN_jump( 
        const uint16_t instruction,
        chip8_cpu_manager_unit& cpu,
        chip8_memory_manager_unit& mmu,
        chip8_screen_manager_unit& smu
    );
    
    echip8_states exec_CXNN_random( 
        const uint16_t instruction,
        chip8_cpu_manager_unit& cpu,
        chip8_memory_manager_unit& mmu,
        chip8_screen_manager_unit& smu
    );
    
    echip8_states exec_DXYN_display( 
        const uint16_t instruction,
        chip8_cpu_manager_unit& cpu,
        chip8_memory_manager_unit& mmu,
        chip8_screen_manager_unit& smu
    );
    
    echip8_states exec_EXGG_skip_if_key(
        const uint16_t instruction,
        chip8_cpu_manager_unit& cpu,
        chip8_memory_manager_unit& mmu,
        chip8_screen_manager_unit& smu
    );
    
    echip8_states exec_FXGG_iomanip( 
        const uint16_t instruction,
        chip8_cpu_manager_unit& cpu,
        chip8_memory_manager_unit& mmu,
        chip8_screen_manager_unit& smu
    );
    
    uint8_t exec_get_key_callback(
        const uint16_t instruction,
        const chip8_cpu_manager_unit& cpu,
        const chip8_memory_manager_unit& mmu
    );

};
