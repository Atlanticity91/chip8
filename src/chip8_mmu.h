#pragma once

#include "chip8_globals.h"

/** 
 * chip8_memory_manager_unit class
 * @note Store and manage memory, call stack, registers and ROM.
 **/
class chip8_memory_manager_unit final {

    static constexpr uint16_t Capacity      = 4096;
    static constexpr uint16_t RegisterCount = 16;
    static constexpr uint16_t StackSize     = 16;

private:
    std::array<uint8_t, Capacity> memory;
    std::array<uint8_t, RegisterCount> registers;
    std::array<uint16_t, StackSize> stack;
    uint8_t stack_top;
    uint16_t rom_size;

public:
    /**
     * Constucor
     **/
    chip8_memory_manager_unit( );

    /**
     * write method
     * @note Write value to memory at targeted address.
     * @param address : Target memory address.
     * @param value : Value to write in the memory.
     **/
    void write( const uint16_t address, const uint8_t value );

    /**
     * push function
     * @note Push address on top of the call stack.
     * @return True when address is adde, false when stack 
     *         is already full.
     **/
    bool push( const uint16_t address );

    /**
     * load_rom function
     * @note Load ROM file to the memory.
     * @param rom_path : Target ROM file path.
     * @return True when ROM load succeded.
     **/
    bool load_rom( const char* rom_path );

    /**
     * dump method
     * @note Dump entire memory content.
     **/
    void dump( );

    /**
     * dump_cpu method
     * @note Dump stack and register content.
     **/
    void dump_cpu( );

private:
    /**
     * dump_registers method
     * @note Dump register content.
     **/
    void dump_registers( );

    /**
     * dump_stack method
     * @note Dump stack content.
     **/
    void dump_stack( );

public:
    /**
     * v function
     * @note Register accessor named v0-vf in chip 8, 
     *       put here for convenience.
     * @param register_id : Target register id from 0 
     *                      to 16 ( 0x0 -> 0xF ).
     * @return Reference to target register.
     **/
    uint8_t& v( const uint8_t register_id );

    /**
     * read function
     * @note Read a byte from targeted memory address.
     * @param address : Target memory address.
     * @return Byte value of targeted memory address.
     **/
    uint8_t read( const uint16_t address ) const;
    
    /**
     * pop function
     * @note Pop first address on top of the call stack.
     * @return A tuple of the top stack address and boolean
     *         true for success or false if try to pop empty stack.
     **/
    std::tuple<uint16_t, bool> pop( );

    /**
     * get_rom_size function
     * @note Return the current loaded ROM size.
     * @return Current loaded ROM size or 0 otherwise.
     **/
    uint16_t get_rom_size( ) const;

    /**
     * fetch function
     * @note Fetch instruction from ROM.
     * @param cpu_pc : Current cpu program counter value.
     * @return Instruction from the ROM for PC.
     **/
    uint16_t fetch( const uint16_t cpu_pc ) const;

};
