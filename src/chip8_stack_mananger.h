#pragma once

#include "chip8_globals.h"

/**
 * chip8_stack_mananger class
 * @note Store and manage call stack.
 **/
class chip8_stack_mananger final {

    static constexpr uint16_t StackSize = 16;

private:
    std::stack<uint16_t> stack;

public:
    /**
     * Constructor
     **/
    chip8_stack_mananger( );

    /**
     * push function
     * @note Push address on top of the call stack.
     * @param address : Target address to push.
     * @param is_unlimited : True when the stack can grow infinitly.
     * @return True when address is adde, false when stack 
     *         is already full.
     **/
    bool push(
        const uint16_t address,
        const bool is_unlimited
    );

    /**
     * dump method
     * @note Dump stack content.
     **/
    void dump( ) const;

public:
    /**
     * pop function
     * @note Pop first address on top of the call stack.
     * @return A tuple of the top stack address and boolean
     *         true for success or false if try to pop empty stack.
     **/
    std::tuple<uint16_t, bool> pop( );

};
