#pragma once

#include "chip8_cpu_timer_manager.h"

struct chip8_cpu_manager_unit;

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
 * chip8_cpu_opcode_manager class
 * @note Store and manage opcocdes.
 **/
class chip8_cpu_opcode_manager final {

    static constexpr uint8_t Count = 16;
    static constexpr chip8_string Unnamed = "~unnamed";

    /**
     * 
     * @note Define a record for cpu opcodes storage.
     **/
    struct chip8_cpu_opcode {
        chip8_opcode exec;
        chip8_string name;
    };

private:
    std::array<chip8_cpu_opcode, Count> opcodes;

public:
    /**
     * Constructor
     **/
    chip8_cpu_opcode_manager( );

    /**
     * set method
     * @note Set opcode implementation.
     * @param opcode : Target opcode.
     * @param opcode_name : Target opcode name.
     * @param implementation : Target instruction implementation 
     *                         function.
     **/
    void set(
        const uint8_t opcode,
        chip8_string opcode_name,
        chip8_opcode&& implementation
    );

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
        chip8_cpu_manager_unit& cpu,
        chip8_memory_manager_unit& mmu,
        chip8_screen_manager_unit& smu
    );

    /**
     * dump method
     * @note Dump all opcodes value and name.
     **/
    void dump( ) const;

private:
    /**
     * initialize method
     * @note Initialize all opcodes to unimplemented.
     **/
    void initialize( );

public:
    /**
     * get_name function
     * @note Get the name for an opcode.
     * @param opcode : Target opcode.
     * @return Target opcode name or "~unnamed" if opcode is
     *         not found or wasn't defined at registration.
     **/
    chip8_string get_name( const uint8_t opcode ) const;

};
