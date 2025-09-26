#include "chip8_cmu.h"

////////////////////////////////////////////////////////////////////////////////////////////
//		===	PUBLIC ===
////////////////////////////////////////////////////////////////////////////////////////////
chip8_screen_manager_unit::chip8_screen_manager_unit( )
    : screen_buffer{ }
{ }

void chip8_screen_manager_unit::clear( ) {
    auto pixel_id = dimenion;

    while ( pixel_id-- > 0 )
        screen_buffer[ pixel_id ] = 0x0;
}

void chip8_screen_manager_unit::display(
    chip8_memory_manager_unit& mmu,
    chip8_cpu_manager_unit& cpu, 
    const chip8_screen_payload& payload
) {
    const auto sx = uint8_t( payload.vx % columns );
    const auto sy = uint8_t( payload.vy % rows );

    mmu.v( 0xF ) = 0x00;

    for ( auto sprite_row = 0; sprite_row < payload.n; sprite_row++ ) {
        const auto sprite = mmu.read( cpu.I + sprite_row );
        const auto py = sy + sprite_row;

        if ( py >= rows )
            return;

        for ( auto pixel_bit = 0; pixel_bit < 8; pixel_bit++ ) {
            const auto px = sx + pixel_bit;
            
            if ( px >= columns )
                break;

            if ( auto sprite_pixel = get_pixel_of_sprite( sprite, pixel_bit ) ) {
                const auto pixel = get_pixel( px, py );

                if ( pixel )
                    mmu.v( 0xF ) = 0x01;

                set_pixel( px, py, pixel ^ sprite_pixel );
            }
        }
    }
}

void chip8_screen_manager_unit::dump( ) {
    auto line = std::array<char, columns+1>{ };

    line[ columns ] = '\0';

    printf( "> Screen :\n" );

    for ( auto y = 0; y < rows; y++ ) {
        for ( auto x = 0; x < columns; x++ )
            line[ x ] = get_pixel( x, y ) ? '#' : '.';

        const auto* line_string = line.data( );

        printf( "%s\n", line_string );
    }
}

////////////////////////////////////////////////////////////////////////////////////////////
//		===	PRIVATE ===
////////////////////////////////////////////////////////////////////////////////////////////
void chip8_screen_manager_unit::set_pixel(
    const uint8_t x,
    const uint8_t y,
    const bool is_on
) {
    const auto pixel_id = get_pixel_id( x, y );

    screen_buffer[ pixel_id ] = is_on;
}

////////////////////////////////////////////////////////////////////////////////////////////
//		===	PUBLIC GET ===
////////////////////////////////////////////////////////////////////////////////////////////
const uint8_t* chip8_screen_manager_unit::get_screen_buffer( ) const {
    return screen_buffer.get( );
}

////////////////////////////////////////////////////////////////////////////////////////////
//		===	PRIVATE GET ===
////////////////////////////////////////////////////////////////////////////////////////////
uint16_t chip8_screen_manager_unit::get_pixel_id(
    const uint8_t x,
    const uint8_t y
) const {
    return uint16_t( y * columns + x );
}

bool chip8_screen_manager_unit::get_pixel_of_sprite(
    const uint8_t sprite,
    const uint8_t pixel_bit
) const {
    const auto offset = 7 - pixel_bit;
    const auto pixel  = sprite >> offset;

    return pixel & 0x01;
}
    
bool chip8_screen_manager_unit::get_pixel(
    const uint8_t x,
    const uint8_t y
) const {
    const auto pixel_id = get_pixel_id( x, y );

    return screen_buffer[ pixel_id];
}
