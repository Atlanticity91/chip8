#pragma once

#include "chip8_cpu_option_manager.h"

/**
 * Define chip8_get_key function signature.
 * @note Any function that match can be use to define 
 *       cpu get_key callback.
 **/
using chip8_get_key = std::function<uint8_t(
    const uint16_t,
    const chip8_cpu_manager_unit&,
    const chip8_memory_manager_unit&
)>;

/**
 * chip8_cpu_manager_unit class
 * @note Store and manage cpu specific logic.
 **/
struct chip8_cpu_manager_unit final {

    uint16_t PC;
    uint16_t I;
    chip8_cpu_timer_manager timers;
    chip8_cpu_opcode_manager opcodes;
    chip8_cpu_option_manager options;
    chip8_get_key get_key_callback;

    /**
     * Constructor
     * @param legacy_mode : True to use this to make vx = vy before 
     *                      shift instructio calls.
     * @param enable_print : True to use printing during execution.
     * @param enable_stack_limit : True to limit call stack to 16
     *                             calls, origninal chip8 limit.
     **/
    chip8_cpu_manager_unit(
        const bool legacy_mode,
        const bool enable_print,
        const bool enable_stack_limit
    );

    /**
     * reset method
     * @note Reset cpu to initial execution state, timers at default 
     *       value and PC and I at 0.
     **/
    void reset( );

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
     * register_op method
     * @note Set opcode implementation.
     * @param opcode : Target opcode.
     * @param opcode_name : Target opcode name.
     * @param implementation : Target instruction implementation 
     *                         function.
     **/
    void register_op(
        const uint8_t opcode,
        chip8_string opcode_name,
        chip8_opcode&& implementation
    );

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
     * set_key_callback method
     * @note Set get key callback.
     * @param callback : Target get key callback.
     **/
    void set_key_callback( chip8_get_key&& callback );

    /**
     * print_instruction method
     * @note Print the instruction with a format, only 
     *       when use_print is true.
     * @param instruction : Target instruction.
     * @param format : Target instruction format.
     **/
    void print_instruction( 
        const uint16_t instruction,
        const echip8_formats format
    ) const;

    /**
     * update_timers method
     * @note Update delay and sound timers.
     **/
    void update_timers( );

    /**
     * consume method
     * @note Consume instruction at PC.
     **/
    void consume( );

    /**
     * execute function
     * @note Execute an instruction.
     * @param instruction : Target instruction.
     * @param mmu : Reference to current memory management unit.
     * @param smu : Reference to current screen management unit.
     * @return Instruction execution state.
     **/
    echip8_states execute(
        const uint16_t instruction,
        chip8_memory_manager_unit& mmu,
        chip8_screen_manager_unit& smu
    );

    /**
     * dump_timers method
     * @note Dump timers values.
     **/
    void dump_timers( ) const;

    /**
     * dump_opcodes method
     * @note Dump opcodes value and names.
     **/
    void dump_opcodes( ) const;

    /**
     * dump_options method
     * @note Dump option value and names.
     **/
    void dump_options( ) const;

    /** 
     * dump_state method
     * @note Dump cpu PC and I value.
     **/
    void dump_state( ) const;

    /**
     * dump method
     * @note Dump cpu information.
     **/
    void dump( chip8_memory_manager_unit& mmu ) const;

    /**
     * get_options function
     * @note Get reference to current option manager instance.
     * @return Reference to current option manager instance.
     **/
    chip8_cpu_option_manager& get_options( );

    /**
     * get_option function
     * @note Get value of an option.
     * @param option : Target option.
     * @return True when the target option is on, false otherwise.
     **/
    bool get_option( const echip8_cpu_options option ) const;

    /**
     * get_opcode_name function
     * @note Get the name for an opcode.
     * @param opcode : Target opcode.
     * @return Target opcode name or "~unnamed" if opcode is
     *         not found or wasn't defined at registration.
     **/
    chip8_string get_opcode_name( const uint8_t opcode ) const;

    /**
     * get_delay_timer function
     * @note Get delay timer value.
     * @return Delay timer value.
     **/
    uint8_t get_delay_timer( ) const;

    /**
     * get_sound_timer function
     * @note Get sound timer value.
     * @return Sound timer value.
     **/
    uint8_t get_sound_timer( ) const;

    /**
     * nibble function
     * @note Get nibble from an instruction.
     * @param target : Target nibble format.
     * @param instruction : Target instruction.
     * @return Nibble from the instruction.
     **/
    uint8_t nibble(
        const echip8_nibbles target,
        const uint16_t instruction
    ) const;

    /**
     * address function
     * @note Get the address nibble from an instruction.
     * @param instruction : Target instruction.
     * @return Address nibble from the instruction.
     **/
    uint16_t address( const uint16_t instruction ) const;

    /**
     * try_map_key function
     * @note Map a key to range [0:15].
     * @param key : Target key to map.
     * @return Target key mapped 0 <= key < 16 or 255 if EOF.
     **/
    uint8_t try_map_key( const uint8_t key ) const;

    /**
     * validate_key function
     * @note Validate if a key is valid.
     * @param key : Target key value to validate.
     * @return True when the key is valid aka ( 0 <= key <= 16 ).
     **/
    bool validate_key( const uint8_t key ) const;

    /**
     * get_key function
     * @note Get key logic execution.
     * @param instruction : Target instruction.
     * @param mmu : Reference to current memory management unit.
     * @return Tuple of key validity and key value in range (0-9,a-b).
     **/
    std::tuple<bool, uint8_t> get_key(
        const uint16_t instruction,
        const chip8_memory_manager_unit& mmu
    ) const;

};
