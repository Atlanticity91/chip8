#pragma once

#include "chip8_cpu_implementation.h"

/**
 * Define all dumping modes possible.
 **/
enum echip8_dump_modes : uint8_t { 
    ecdm_screen = 0,
    ecdm_memory,
    ecdm_cpu,
    ecdm_timers,
    ecdm_opcodes,
    ecdm_options,
    ecdm_state,
    ecdm_font,
    ecdm_rom,
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
    chip8_rom_manager_unit rom;

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
     * reset method
     * @note Reset memory and screen to initial state.
     **/
    void reset( );

    /** 
     * reset_opcodes method
     * @note Reset cpu opcodes to initial state.
     **/
    void reset_opcodes( );

    /**
     * reset_get_key method
     * @note Reset cpu get key callback to initial state.
     **/
    void reset_get_key( );

    /** 
     * override_opcode method
     * @note Override instruction implementation.
     * @param opcode : Target opcode to override.
     * @param opcode_name : Target new opcode name.
     * @param implementation : Target new opcode implementation.
     **/
    void override_opcode(
        const uint8_t opcode,
        chip8_string opcode_name,
        chip8_opcode&& implementation
    );

    /**
     * override_key_callback method
     * @note Set get key callback.
     * @param callback : Target get key callback.
     **/
    void override_key_callback( chip8_get_key&& callback );

    /**
     * set_delay_timer method
     * @note Set delay timer value.
     * @param value : Target delay timer value.
     **/
    void set_delay_timer( const uint8_t value );

    /**
     * set_sound_timer method
     * @note Set sound timer value.
     * @param value : Target sound timer value.
     **/
    void set_sound_timer( const uint8_t value );

    /**
     * set_option method
     * @note Set cpu option.
     * @param option : Target option to update.
     * @param value : Target option value.
     **/
    void set_option(
        const echip8_cpu_options option,
        const bool value
    );

    /**
     * load_rom function
     * @note Load ROM file to the memory.
     * @param rom_path : Target ROM file path.
     * @return True when ROM load succeded.
     **/
    bool load_rom( chip8_string rom_path );
    
    /**
     * execute function
     * @note Execute the currently stored ROM.
     * @param instruction_per_second : Maximum instruction execution
     *                                 per second.
     * @return Emulateur state at the end of ROM execution.
     **/
    echip8_states execute(
        const uint32_t instruction_per_second = 700
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
        chip8_string rom_path,
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
     * get_mmu function
     * @note Get reference to current memory manager unit.
     * @return Reference to current memory manager unit.
     **/
    chip8_memory_manager_unit& get_mmu( );

    /**
     * get_smu function
     * @note Get reference to current screen manager unit.
     * @return Reference to current screen manager unit.
     **/
    chip8_screen_manager_unit& get_smu( );

    /**
     * get_cpu function
     * @note Get reference to current cpu manager unit.
     * @return Reference to current cpu manager unit.
     **/
    chip8_cpu_manager_unit& get_cpu( );
    
    /**
     * get_rom function
     * @note Get reference to current rom manager unit.
     * @return Reference to current rom manager unit.
     **/
    chip8_rom_manager_unit& get_rom( );

    /**
     * get_screen_buffer function
     * @note Get access to screen buffer.
     * @return Pointer to imutable screen buffer.
     **/
    const uint8_t* get_screen_buffer( ) const;

};
