#include "chip8.h"

////////////////////////////////////////////////////////////////////////////////////////////
//		===	PUBLIC ===
////////////////////////////////////////////////////////////////////////////////////////////
chip8_memory_manager_unit::chip8_memory_manager_unit( )
    : memory{ },
    registers{ },
    stack{ }
{
    reset( );
}

void chip8_memory_manager_unit::reset( ) {
    const uint8_t font[] = {
        0xF0, 0x90, 0x90, 0x90, 0xF0, // 0
        0x20, 0x60, 0x20, 0x20, 0x70, // 1
        0xF0, 0x10, 0xF0, 0x80, 0xF0, // 2
        0xF0, 0x10, 0xF0, 0x10, 0xF0, // 3
        0x90, 0x90, 0xF0, 0x10, 0x10, // 4
        0xF0, 0x80, 0xF0, 0x10, 0xF0, // 5
        0xF0, 0x80, 0xF0, 0x90, 0xF0, // 6
        0xF0, 0x10, 0x20, 0x40, 0x40, // 7
        0xF0, 0x90, 0xF0, 0x90, 0xF0, // 8
        0xF0, 0x90, 0xF0, 0x10, 0xF0, // 9
        0xF0, 0x90, 0xF0, 0x90, 0x90, // A
        0xE0, 0x90, 0xE0, 0x90, 0xE0, // B
        0xF0, 0x80, 0x80, 0x80, 0xF0, // C
        0xE0, 0x90, 0x90, 0x90, 0xE0, // D
        0xF0, 0x80, 0xF0, 0x80, 0xF0, // E
        0xF0, 0x80, 0xF0, 0x80, 0x80  // F
    };

    auto* font_memory = get_font_memory( );

    std::memmove( font_memory, font, sizeof( font ) );
}

void chip8_memory_manager_unit::write(
    const uint16_t address,
    const uint8_t value 
) {
    memory[ address ] = value;
}

bool chip8_memory_manager_unit::push(
    const uint16_t address,
    const bool is_unlimited
) {
    return stack.push( address, is_unlimited );
}

void chip8_memory_manager_unit::dump( ) const {
    printf( "> Memory :\n       | 00 01 02 03 04 05 06 07 08 09 0A 0B 0C 0D 0E 0F |\n" );

    print_memory( eca_null, Capacity );
}

void chip8_memory_manager_unit::dump_cpu( ) const {
    dump_registers( );
    stack.dump( );
}

void chip8_memory_manager_unit::dump_font( ) const {
    constexpr auto font_length = eca_font_stop - eca_font_start;

    print_memory( eca_font_start, font_length );
}

void chip8_memory_manager_unit::dump_rom( const uint16_t rom_size ) const {
    printf( "> ROM :\n" );

    if ( rom_size == 0 )
        return;

    print_memory( eca_rom_start, rom_size );
}

////////////////////////////////////////////////////////////////////////////////////////////
//		===	PRIVATE ===
////////////////////////////////////////////////////////////////////////////////////////////
void chip8_memory_manager_unit::dump_registers( ) const {
    printf( "> Regisrers :\n" );
    
    auto idx = 0;

    for ( const auto regiser : registers )
        printf( "v[ %X ] %2X\n", idx++, regiser );
}

void chip8_memory_manager_unit::print_memory(
    const uint16_t address,
    const uint16_t length
) const {
    constexpr auto hex_count = 16;
    constexpr auto hex       = "0123456789ABCDEF";

    const auto memory_stop = std::min( uint16_t( address + length ), Capacity );
    auto memory_address    = address;

    chip8_print_buffer( "0x0000 | 00 01 02 03 04 05 06 07 08 09 0A 0B 0C 0D 0E 0F |" );

    auto in_range = [ memory_address, memory_stop ]( const uint16_t byte_id ) -> bool {
        return memory_address + byte_id < memory_stop;
    };

    while ( memory_address < memory_stop ) {
        auto column_id = 2;

        buffer[ column_id++ ] = hex[ ( memory_address >> 12 ) & 0xF ];
        buffer[ column_id++ ] = hex[ ( memory_address >>  8 ) & 0xF ];
        buffer[ column_id++ ] = hex[ ( memory_address >>  4 ) & 0xF ];
        buffer[ column_id++ ] = hex[ memory_address & 0xF ];

        column_id += 3;

        for ( auto i = 0; i < hex_count && in_range( i ); i++ ) {
            const auto value = memory[ memory_address + i ];

            buffer[ column_id++ ] = hex[ ( value >> 4 ) & 0xF ];
            buffer[ column_id++ ] = hex[ value & 0xF ];

            column_id += 1;
        }

        memory_address += hex_count;
        
        printf( "%s\n", buffer_str );
    }
}

////////////////////////////////////////////////////////////////////////////////////////////
//		===	PUBLIC GET ===
////////////////////////////////////////////////////////////////////////////////////////////
uint8_t* chip8_memory_manager_unit::get_font_memory( ) const {
    return (uint8_t*)&memory[ eca_font_start ];
}

uint8_t* chip8_memory_manager_unit::get_rom_memory( ) const{
    return (uint8_t*)&memory[ eca_rom_start ];
}

uint8_t& chip8_memory_manager_unit::v( const uint8_t register_id ) {
    return registers[ register_id ];
}

uint8_t chip8_memory_manager_unit::read( const uint16_t address ) const {
    return memory[ address ];
}
    
std::tuple<uint16_t, bool> chip8_memory_manager_unit::pop( ) {
    return stack.pop( );
}
