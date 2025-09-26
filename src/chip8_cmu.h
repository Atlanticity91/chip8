#pragma once

#include "chip8_cpu_implementation.h"

/** 
 * Define chip8_opcode function signature.
 * @note Any function that match it can be use for
 *       instruction execution.
 **/
using chip8_opcode = std::function<echip8_states( 
    const uint16_t,
    chip8_cpu_manager_unit&,
    chip8_memory_manager_unit&,
    chip8_screen_manager_unit&
)>;

/**
 * chip8_cpu_manager_unit class
 * @note Store and manage cpu specific logic.
 **/
struct chip8_cpu_manager_unit final {

    bool use_legacy;
    bool use_print;
    uint16_t PC;
    uint16_t I;
    std::atomic<uint8_t> delay_timer;
    std::atomic<uint8_t> sound_timer;
    std::array<chip8_opcode, 16> opcodes;
    std::array<const char*, 16> opcode_names;

    /**
     * Constructor
     * @param legacy_mode : True to use this to make vx = vy before 
     *                      shift instructio calls.
     * @param enable_print : True to use printing during execution.
     **/
    chip8_cpu_manager_unit(
        const bool legacy_mode,
        const bool enable_print
    );

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
        const char* opcode_name,
        chip8_opcode&& implementation
    );

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
    );

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
     * dump method
     * @note Dump cpu information.
     **/
    void dump( chip8_memory_manager_unit& mmu );

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

};
