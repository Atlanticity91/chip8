#include "chip8.h"

////////////////////////////////////////////////////////////////////////////////////////////
//		===	PUBLIC ===
////////////////////////////////////////////////////////////////////////////////////////////
chip8_memory_manager_unit::chip8_memory_manager_unit( )
    : memory{ },
    registers{ },
    stack{ },
    stack_top{ 0 }
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

bool chip8_memory_manager_unit::push( const uint16_t address ) {
    if ( stack_top < uint8_t( stack.size( ) ) ) {
        stack[ stack_top++ ] = address;

        return true;
    }

    return false;
}

void chip8_memory_manager_unit::dump( ) {
    const auto hex_count = 16;
    const auto hex = "0123456789ABCDEF";
    auto line = std::array<char, 59>{ "0x0000 | 00 01 02 03 04 05 06 07 08 09 0A 0B 0C 0D 0E 0F |" };
    const auto* line_string = line.data( );

    printf( "> Memory :\n%s\n", line_string );

    for ( auto address = 0; address < Capacity; address += hex_count ) {
        auto column_id = 2;

        line[ column_id++ ] = hex[ ( address >> 12 ) & 0xF ];
        line[ column_id++ ] = hex[ ( address >>  8 ) & 0xF ];
        line[ column_id++ ] = hex[ ( address >>  4 ) & 0xF ];
        line[ column_id++ ] = hex[ address & 0xF ];

        column_id += 3;

        for ( auto i = 0; i < hex_count; i++ ) {
            const auto value = memory[ address + i ];

            line[ column_id++ ] = hex[ ( value >> 4 ) & 0xF ];
            line[ column_id++ ] = hex[ value & 0xF ];

            column_id += 1;
        }

        printf( "%s\n", line_string );
    }
}

void chip8_memory_manager_unit::dump_cpu( ) {
    dump_registers( );
    dump_stack( );
}

////////////////////////////////////////////////////////////////////////////////////////////
//		===	PRIVATE ===
////////////////////////////////////////////////////////////////////////////////////////////
void chip8_memory_manager_unit::dump_registers( ) {
    printf( "> Regisrers :\n" );
    
    auto idx = 0;

    for ( const auto regiser : registers )
        printf( "v[ %X ] %2X\n", idx++, regiser );
}

void chip8_memory_manager_unit::dump_stack( ) {
    printf( "> Stack (%2d/%d) :\n", stack_top, StackSize );

    auto stack_id = 0;

    while ( stack_id < StackSize )
        printf( "[ %2d ] 0x%4X\n", stack_id, stack[ stack_id++ ] );
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
    if ( stack_top > 0 )
        return std::make_tuple( stack[ --stack_top ], true );

    return std::make_tuple( eca_null, false );
}
