#pragma once

#include "chip8_bitset.h"

struct chip8_cpu_manager_unit;

/** 
 * Define payload for display.
 * @field vx : Draw x position from register vx.
 * @field vy : Draw y position from register vy.
 * @field n : Height of the sprite to draw.
 **/
struct chip8_screen_payload {
    const uint8_t vx;
    const uint8_t vy;
    const uint8_t n;
};

/**
 * Define a screen manager unit
 * @note Store and manage screen buffer.
 **/
class chip8_screen_manager_unit final {

    static constexpr uint16_t columns  = 64;
    static constexpr uint16_t rows     = 32;
    static constexpr uint16_t dimenion = ( columns * rows );

private:
    chip8_bitset<dimenion> screen_buffer;

public:
    /**
     * Constructor 
     **/
    chip8_screen_manager_unit( );

    /**
     * clear method
     * @note Clear the screen buffer.
     **/
    void clear( );

    /**
     * display method
     * @note Display the payload on the screen buffer.
     * @param mmu : Reference to currrent memory manager unit.
     * @param cpu : Reference to current cpu manager unit.
     * @param payload : Target payload to display.
     **/
    void display(
        chip8_memory_manager_unit& mmu,
        chip8_cpu_manager_unit& cpu,
        const chip8_screen_payload& payload
    );

    /**
     * dump method
     * @note Dump the screen buffer content.
     **/
    void dump( );

private:
    /**
     * set_pixel method
     * @note Set the state of the pixel at ( x, y ) 
     *       on screen buffer.
     * @param x : Target pixel x position on screen ( 0 - 63 ).
     * @param y : Target pixel y position on screen ( 0 - 31 ).
     * @param is_on : True to set the pixel on.
     **/
    void set_pixel(
        const uint8_t x,
        const uint8_t y,
        const bool is_on
    );

public:
    /**
     * get_screen_buffer function
     * @note Get access to screen buffer.
     * @return Pointer to imutable screen buffer.
     **/
    const uint8_t* get_screen_buffer( ) const;

private:
    /**
     * get_pixel_id function
     * @note Get pixel index of ( x, y ) on the screen buffer.
     * @param x : Target pixel x position on screen ( 0 - 63 ).
     * @param y : Target pixel y position on screen ( 0 - 31 ).
     * @return Index of the pixel inside the screen buffer.  
     **/
    uint16_t get_pixel_id(
        const uint8_t x,
        const uint8_t y
    ) const;

    /**
     * get_pixel_of_sprite function
     * @note Extract pixel state from sprite.
     * @param sprite : Target sprite.
     * @param pixel_bit : Target sprite bit ( 0 - 8 ).
     * @return True when the target pixel is on, false otherwise.
     **/
    bool get_pixel_of_sprite(
        const uint8_t sprite,
        const uint8_t pixel_bit
    ) const;
    
    /**
     * get_pixel function
     * @note Extract pixel state from screen buffer.
     * @param x : Target pixel x position on screen ( 0 - 63 ).
     * @param y : Target pixel y position on screen ( 0 - 31 ).
     * @return True when the target pixel is on, false otherwise. 
     **/
    bool get_pixel(
        const uint8_t x,
        const uint8_t y
    ) const;

};
