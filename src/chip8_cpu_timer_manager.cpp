#include "chip8.h"

////////////////////////////////////////////////////////////////////////////////////////////
//		===	PUBLIC ===
////////////////////////////////////////////////////////////////////////////////////////////
chip8_cpu_timer_manager::chip8_cpu_timer_manager( )
{ 
    reset( ); 
}


void chip8_cpu_timer_manager::set_make_noise( chip8_make_noise_callback&& callback ) {
    user_make_noise = std::move( callback );
}

void chip8_cpu_timer_manager::reset( ) {
    delay_timer = 60;
    sound_timer = 60;
}

void chip8_cpu_timer_manager::set_delay( const uint8_t value ) {
    delay_timer = value;
}

void chip8_cpu_timer_manager::set_sound( const uint8_t value ) {
    sound_timer = value;
}

void chip8_cpu_timer_manager::update( ) {
    if ( delay_timer > 0 )
        delay_timer -= 1;

    if ( sound_timer > 0 ) {
        sound_timer -= 1;

        invoke_make_noise( );
    }
}

void chip8_cpu_timer_manager::dump( ) const {
    const auto delay_value = delay_timer.load( );
    const auto sound_value = sound_timer.load( );

    printf( "Delay Timer %3d\nSound Timer %3d\n", delay_value, sound_value );
}

////////////////////////////////////////////////////////////////////////////////////////////
//		===	PRIVATE ===
////////////////////////////////////////////////////////////////////////////////////////////
void chip8_cpu_timer_manager::invoke_make_noise( ) {
    if ( !user_make_noise )
        return;

    std::invoke( user_make_noise );
}

////////////////////////////////////////////////////////////////////////////////////////////
//		===	PUBLIC GET ===
////////////////////////////////////////////////////////////////////////////////////////////
uint8_t chip8_cpu_timer_manager::get_delay( ) const {
    return delay_timer;
}

uint8_t chip8_cpu_timer_manager::get_sound( ) const {
    return sound_timer;
}
