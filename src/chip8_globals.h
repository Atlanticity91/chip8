#pragma once

#include <array>
#include <cinttypes>
#include <chrono>
#include <cstdio>
#include <cstdlib>
#include <filesystem>
#include <functional>
#include <fstream>
#include <thread>
#include <tuple>

/**
 * Define self macro, syntatic sugar.
 **/
#define chip8_self (*this)

/**
 * Define helper for printing buffer creation.
 * @param FORMAT : Target printing format.
 **/
#define chip8_print_buffer( FORMAT )\
    auto buffer = std::array<char, sizeof( FORMAT )>{ FORMAT };\
    auto* buffer_str = buffer.data( )

/**
 * Define string type, syntatic sugar.
 **/
using chip8_string = const char*;

/**
 * Define all possible state of the emulator
 **/
enum echip8_states : uint8_t {
    ecs_run = 0,
    ecs_eop, // End Of Program
    ecs_nip, // No Input ROM
    ecs_iir, // Invalid Input ROM
    ecs_uop, // Unimplemented OPcode
    ecs_sgf, // Segmentation Fault
    ecs_iik, // Invalid Input Key
};

/**
 * Define all possible nibbles for instructions.
 **/
enum echip8_nibbles : uint8_t {
    ecn_x = 0,
    ecn_y,
    ecn_n,
    ecn_nn
};

/**
 * Define all possible nibbles masks for instructions.
 **/
enum echip8_masks : uint16_t {
    ecm_x   = 0x0F00,
    ecm_y   = 0x00F0,
    ecm_n   = 0x000F,
    ecm_nn  = 0x00FF,
    ecm_nnn = 0x0FFF
};

/** 
 * Define handful memory address.
 **/
enum echip8_addresses : uint16_t {
    eca_null       = 0x0000,
    eca_font_start = 0x0050,
    eca_font_stop  = 0x009F,
    eca_rom_start  = 0x0200
};

/**
 * Define all possible formats or nibbles for
 * instruction printing.
 **/
enum echip8_formats : uint8_t {
    ecf_xnn = 0,
    ecf_xyn,
    ecf_nnn
};

/**
 * Define all possible cpu options.
 **/
enum echip8_cpu_options : uint8_t {
    ecc_option_legacy = 0,
    ecc_option_print,
    ecc_option_count
};
