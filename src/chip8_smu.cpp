#include "chip8.h"

////////////////////////////////////////////////////////////////////////////////////////////
//		===	PUBLIC ===
////////////////////////////////////////////////////////////////////////////////////////////
chip8_screen_manager_unit::chip8_screen_manager_unit( )
    : screen_buffer{ }
{ }

void chip8_screen_manager_unit::set_clear_callback(
    chip8_display_clear_callback&& callback
) {
    user_clear = std::move( callback );
}

void chip8_screen_manager_unit::set_draw_callback(
    chip8_display_draw_callback&& callback
) {
    user_draw = std::move( callback );
}

void chip8_screen_manager_unit::clear( ) {
    auto pixel_id = dimenion;

    while ( pixel_id-- > 0 )
        screen_buffer[ pixel_id ] = 0x0;

    if ( user_clear )
        std::invoke( user_clear );
}

void chip8_screen_manager_unit::display(
    chip8_memory_manager_unit& mmu,
    chip8_cpu_manager_unit& cpu, 
    const chip8_screen_payload& payload
) {
    const auto screen_x = uint8_t( payload.vx % columns );
    const auto screen_y = uint8_t( payload.vy % rows );

    mmu.v( 0xF ) = 0x00;

    for ( auto sprite_row = 0; sprite_row < payload.n; sprite_row++ ) {
        const auto position_y = uint8_t( screen_y + sprite_row );
        const auto sprite     = mmu.read( cpu.I + sprite_row );

        if ( position_y >= rows )
            return;

        draw_sprite( mmu, { sprite, screen_x, position_y } );
    }

    invoke_user_draw( );
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

void chip8_screen_manager_unit::draw_sprite(
    chip8_memory_manager_unit& mmu,
    const chip8_sprite_payload& sprite_payload
) {
    for ( auto pixel_bit = 0; pixel_bit < 8; pixel_bit++ ) {
        const auto px = sprite_payload.screen_x + pixel_bit;
            
        if ( px >= columns )
            break;

        if ( auto sprite_pixel = get_pixel_of_sprite( sprite_payload.sprite, pixel_bit ) ) {
            const auto pixel = get_pixel( px, sprite_payload.position_y );

            mmu.v( 0xF ) = 0x01 * pixel;

            set_pixel( px, sprite_payload.position_y, pixel ^ sprite_pixel );
        }
    }
}

void chip8_screen_manager_unit::invoke_user_draw( ) {
    if ( !user_draw ) 
        return;

    const auto screen_height = uint8_t( rows );
    const auto screen_width  = uint8_t( columns );
    const auto* pixel_pool   = screen_buffer.get( );

    std::invoke( user_draw, pixel_pool, screen_width, screen_height );
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
