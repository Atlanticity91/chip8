#pragma once

#include "chip8_cmu.h"

/**
 * Define all dumping modes possible.
 **/
enum echip8_dump_modes : uint8_t { 
    ecdm_screen = 0,
    ecdm_memory,
    ecdm_cpu,
    ecdm_all
};

/**
 * chip8 class
 * @note Wrapper for all component to emulate chip8.
 **/
class chip8 final {
    
    using clock_t = std::chrono::steady_clock;

private:
    chip8_memory_manager_unit mmu;
    chip8_screen_manager_unit smu;
    chip8_cpu_manager_unit cpu;

public:
    /**
     * Constructor
     * @param legacy_mode : True to use this to make vx = vy before 
     *                      shift instructio calls.
     * @param enable_print : True to use printing during execution.
     **/
    chip8( 
        const bool legacy_mode = false,
        const bool enable_print = true 
    );

    /** 
     * override method
     * @note Override instruction implementation.
     * @param opcode : Target opcode to override.
     * @param opcode_name : Target new opcode name.
     * @param implementation : Target new opcode implementation.
     **/
    void override(
        const uint8_t opcode,
        const char* opcode_name,
        chip8_opcode&& implementation
    );
    
    /**
     * execute function
     * @note Execute a rom with specified instruction per 
     *       second limit.
     * @param rom_path : Target ROM path.
     * @param instruction_per_second : Maximum instruction execution
     *                                 per second.
     * @return Emulateur state at the end of ROM execution.
     **/
    echip8_states execute( 
        const char* rom_path,
        const uint32_t instruction_per_second = 700
    );

    /**
     * dump method
     * @note Dump all content for the target mode.
     * @param mode : Targte dump mode.
     **/
    void dump( const echip8_dump_modes mode = ecdm_screen );

    /**
     * print_exec_state method
     * @note Pretty print for target execution return.
     * @param exec_state : Target execution state.
     **/
    void print_exec_state( const echip8_states exec_state );

private:
    /**
     * try_wait method
     * @note Try to wait some time to execute instruction
     *       at the "correct" chip8 speed ~( 400Hz - 800Hz ).
     * @param instruction_per_second : Maximum instruction execution
     *                                 per second.
     * @param instruction_counter : Executed instruction count since
     *                              last try_wait call.
     * @param cycle_start : Timepoint that represent the start of 
     *                      current execution sycle sinde last try_wait.
     **/
    void try_wait(
        const uint32_t instruction_per_second,
        int& instruction_counter,
        clock_t::time_point& cycle_start
    );

public:
    /**
     * get_screen_buffer function
     * @note Get access to screen buffer.
     * @return Pointer to imutable screen buffer.
     **/
    const uint8_t* get_screen_buffer( ) const;

};
