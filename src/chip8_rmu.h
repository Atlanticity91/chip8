#pragma once

#include "chip8_mmu.h"

/**
 * chip8_rom class
 * @note Store and manage ROM.
 **/
class chip8_rom_manager_unit final {

private:
    uint16_t size;
    chip8_string path;

public:
    /**
     * Constructor
     **/
    chip8_rom_manager_unit( );

    /**
     * load function
     * @note Load ROM file to the memory.
     * @param mpu : Reference to current memory manager unit.
     * @param rom_path : Target ROM file path.
     * @return True when ROM load succeded.
     **/
    bool load( 
        chip8_memory_manager_unit& mmu,
        chip8_string rom_path
    );

    /**
     * dump method
     * @note Dump the entire ROM content.
     * @param mpu : Reference to current memory manager unit.
     **/
    void dump( const chip8_memory_manager_unit& mmu ) const;

public:
    /**
     * exist function
     * @note Get if a ROM is loaded in memory.
     * @return True when ROM is loaded, false otherwise.
     **/
    bool exist( ) const;

    /**
     * get function
     * @note Get current rom memory space.
     * @param mpu : Reference to current memory manager unit.
     * @return Return first and last rom memory space address.
     **/
    std::tuple<uint8_t*, uint8_t*> get( 
        const chip8_memory_manager_unit& mmu
    ) const;

    /**
     * get_rom_size function
     * @note Return the current loaded ROM size.
     * @return Current loaded ROM size or 0 otherwise.
     **/
    uint16_t get_size( ) const;

    /**
     * get_path function
     * @note Get current loaded ROM path.
     * @return ROM path or empty string.
     **/
    chip8_string get_path( ) const;

    /**
     * fetch function
     * @note Fetch instruction from ROM.
     * @param mpu : Reference to current memory manager unit.
     * @param cpu_pc : Current cpu program counter value.
     * @return Instruction from the ROM for PC.
     **/
    uint16_t fetch(
        chip8_memory_manager_unit& mmu,
        const uint16_t cpu_pc
    ) const;

};
