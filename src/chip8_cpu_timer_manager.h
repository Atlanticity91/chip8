#pragma once

#include "chip8_smu.h"

/**
 * chip8_cpu_timer_manager class
 * @note Store and manage chip8 timers.
 **/
class chip8_cpu_timer_manager final {

private:
    std::atomic<uint8_t> delay_timer;
    std::atomic<uint8_t> sound_timer;
    chip8_make_noise_callback user_make_noise;

public:
    /**
     * Constructor
     **/
    chip8_cpu_timer_manager( );

    /**
     * set_make_noise method
     * @note Set make noise callback.
     * @param callback : Target callback.
     **/
    void set_make_noise( chip8_make_noise_callback&& callback );

    /**
     * reset method
     * @note Reset delay and sound timers to initial value. 
     **/
    void reset( );

    /**
     * set_delay method
     * @note Set delay timer value.
     * @param value : Target delay timer value.
     **/
    void set_delay( const uint8_t value );

    /**
     * set_sound method
     * @note Set sound timer value.
     * @param value : Target sound timer value.
     **/
    void set_sound( const uint8_t value );

    /**
     * update method
     * @note Update delay and sound timers.
     **/
    void update( );

    /**
     * dump method
     * @note Dump timers values.
     **/
    void dump( ) const;

private:
    /**
     * invoke_make_noise method
     * @note Proxy for invoking make noise callback.
     **/
    void invoke_make_noise( );

public:
    /**
     * get_delay function
     * @note Get delay timer value.
     * @return Delay timer value.
     **/
    uint8_t get_delay( ) const;

    /**
     * get_sound function
     * @note Get sound timer value.
     * @return Sound timer value.
     **/
    uint8_t get_sound( ) const;

};
