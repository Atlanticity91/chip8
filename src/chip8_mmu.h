#pragma once

#include "chip8_globals.h"

// TODO Add unlimited call stack mode.

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

public:
    /**
     * Constucor
     **/
    chip8_memory_manager_unit( );

    /**
     * reset method
     * @note Reset memory to initial state.
     **/
    void reset( );

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
     * dump method
     * @note Dump entire memory content.
     **/
    void dump( ) const;

    /**
     * dump_cpu method
     * @note Dump stack and register content.
     **/
    void dump_cpu( ) const;

    /**
     * dump_font method
     * @note Dump font data from memory.
     **/
    void dump_font( ) const;

    /**
     * dump_rom method
     * @note Dump ROM data from memory.
     * @param rom_size : Target bytes to print from ROM.
     **/
    void dump_rom( const uint16_t rom_size ) const;

private:
    /**
     * dump_registers method
     * @note Dump register content.
     **/
    void dump_registers( ) const;

    /**
     * dump_stack method
     * @note Dump stack content.
     **/
    void dump_stack( ) const;

    /**
     * print_memory method
     * @note Print formated memory content.
     * @param address : Target memory starting address.
     * @param length : Target memory buffer length.
     **/
    void print_memory(
        const uint16_t address,
        const uint16_t length
    ) const;

public:
    /**
     * get_font_memory function
     * @note Get font memory address.
     * @return Return font memory address.
     **/
    uint8_t* get_font_memory( ) const;
    
    /**
     * get_rom_memory function
     * @note Get ROM memory address.
     * @return Return ROM memory address.
     **/
    uint8_t* get_rom_memory( ) const;

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

};
