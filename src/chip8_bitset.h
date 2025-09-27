#pragma once

#include "chip8_rmu.h"

/**
 * chip8_bitset template class
 * @note Partial bitset implementation just for memory 
 *       management and external accessor.
 **/
template<uint16_t Capacity>
class chip8_bitset final {

    static constexpr uint16_t dimension = Capacity / 8;

    /**
     * chip8_bitset_proxy class
     * @note Define for operator[] hacking.
     **/
    class chip8_bitset_proxy final {

    private:
        chip8_bitset* bitset;
        uint16_t bit_id;
        bool bit_value;

    public:
        /**
         * Constructor
         * @param value : Target bit value.
         **/
        chip8_bitset_proxy( const bool value )
            : bitset{ nullptr },
            bit_id{ 0 },
            bit_value{ value }
        { };

        /**
         * Constructor
         * @param bitset : Pointer to current bitset.
         * @param bit_id : Target bit to set.
         **/
        chip8_bitset_proxy(
            chip8_bitset* bitset,
            const uint16_t bit_id
        )
            : bitset{ bitset },
            bit_id{ bit_id },
            bit_value{ false }
        { };

    public:
        /**
         * operator=
         * @note Actual operator[] hack execution.
         * @param other : Proxy that store target bit value.
         * @return Reference to current proxy.
         **/
        chip8_bitset_proxy& operator=(
            const chip8_bitset_proxy& other 
        ) {
            if ( bitset )
                bitset->set_bit( bit_id, other.bit_value );

            return *this;
        };

    };

private:
    std::array<uint8_t, dimension> buffer;

public:
    /**
     * Constructor
     **/
    chip8_bitset( ) 
        : buffer{ 0 }
    { };

private:
    /**
     * set_bit method
     * @note Set a bit of the bitset to a specified value.
     * @param bit_id : Target bit to set.
     * @param value : Target bit value.
     **/
    void set_bit(
        const uint16_t bit_id,
        const bool value
    ) {
        const auto [ byte_id, offset ] = get_coords( bit_id );

        if ( value )
            buffer[ byte_id ] |= ( 1 << offset );
        else
            buffer[ byte_id ] &= ~( 1 << offset );
    };

public:
    /**
     * get function
     * @note Get a pointer to the data buffer.
     * @return Pointer to immutable data buffer.
     **/
    const uint8_t* get( ) const {
        return buffer.data( );
    };

public:
    /**
     * operator[]
     * @note Trick to have bitset[ bit_id ] = true syntax.
     * @param bit_id : Target bit to set.
     * @return Proxy for bit assignement operation.
     **/
    chip8_bitset_proxy operator[]( uint16_t bit_id ) {
        return { this, bit_id };
    };

    /**
     * operator[]
     * @note Get state of a bit in bitset.
     * @param bit_id : Target bit to get.
     * @return True when the bit is 1 or false otherwise.
     **/
    bool operator[]( uint16_t bit_id ) const {
        const auto [ byte_id, offset ] = get_coords( bit_id );

        return ( buffer[ byte_id ] >> offset ) & 0x01;
    };

private:
    /**
     * get_coords function
     * @note Convert target bit index to byte index + offset.
     * @param bit_id : Target bit index.
     * @return Tuple of both byte index + byte bits offset to 
     *         acess it.
     **/
    std::tuple<uint16_t, uint16_t> get_coords(
        const uint16_t bit_id
    ) const {
        return { bit_id / 8, bit_id % 8 };
    };

};
