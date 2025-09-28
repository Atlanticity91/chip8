#pragma once

#include "chip8_cpu_opcode_manager.h"

/**
 * chip8_cpu_option_manager class 
 * @note Store and manage cpu options.
 **/
class chip8_cpu_option_manager final {

    /**
     * chip8_cpu_option struct
     * @note Define a record for option storage.
     **/
    struct chip8_cpu_option {

        chip8_string name;
        bool value;

        /**
         * Constructor
         * @param option_name: Target option name.
         **/
        chip8_cpu_option( chip8_string option_name )
            : name{ option_name },
            value{ false }
        { };

    };

private:
    std::array<chip8_cpu_option, ecc_option_count> options;

public:
    /**
     * Constructor 
     **/
    chip8_cpu_option_manager( );

    /**
     * set method
     * @note Set cpu option.
     * @param option : Target option to update.
     * @param value : Target option value.
     **/
    void set(
        const echip8_cpu_options option,
        const bool value
    );

    /**
     * dump method
     * @note Dump all option value and name.
     **/
    void dump( ) const;

public:
    /**
     * get function
     * @note Get value of an option.
     * @param option : Target option.
     * @return True when the target option is on, false otherwise.
     **/
    bool get( const echip8_cpu_options option ) const;

public:
    /**
     * operator[]
     * @note Operator for getting value of an option.
     * @param option : Target option.
     * @return True when the target option is on, false otherwise.
     **/
    bool operator[]( const echip8_cpu_options option ) const;

};
