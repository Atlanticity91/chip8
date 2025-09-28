#include "chip8.h"

////////////////////////////////////////////////////////////////////////////////////////////
//		===	PUBLIC ===
////////////////////////////////////////////////////////////////////////////////////////////
chip8_cpu_manager_unit::chip8_cpu_manager_unit(
    const bool legacy_mode,
    const bool enable_print,
    const bool enable_stack_limit
)
    : PC{ 0 },
    I{ 0 },
    timers{ },
    opcodes{ },
    options{ }
{
    set_option( ecc_option_legacy, legacy_mode );
    set_option( ecc_option_print, enable_print );
    set_option( ecc_option_stack, !enable_stack_limit );
}

void chip8_cpu_manager_unit::reset( ) {
    PC = 0;
    I  = 0;

    timers.reset( );
}

void chip8_cpu_manager_unit::set_delay_timer( const uint8_t value ) {
    timers.set_delay( value );
}

void chip8_cpu_manager_unit::set_sound_timer( const uint8_t value ) {
    timers.set_sound( value );
}

void chip8_cpu_manager_unit::register_op(
    const uint8_t opcode,
    chip8_string opcode_name,
    chip8_opcode&& implementation
) {
    opcodes.set( opcode, opcode_name, std::move( implementation ) );
}

void chip8_cpu_manager_unit::set_option(
    const echip8_cpu_options option,
    const bool value
) {
    options.set( option, value );
}

void chip8_cpu_manager_unit::set_key_callback( chip8_get_key&& callback ) {
    if ( callback )
        get_key_callback = std::move( callback );
}

void chip8_cpu_manager_unit::print_instruction( 
    const uint16_t instruction,
    const echip8_formats format
) const {
    if ( !options[ ecc_option_print ] )
        return;

    const auto line_size = 31;
    const auto opcode    = uint8_t( instruction >> 12 );

    chip8_print_buffer( "0000 | 0x0 0x000 0x000 0x000 |" );
    
    auto x = uint16_t( nibble( ecn_x, instruction ) );
    auto y = uint16_t( nibble( ecn_y, instruction ) );
    auto n = uint16_t( nibble( ecn_n, instruction ) );

    switch ( format ) {
        case ecf_xnn : 
            y = nibble( ecn_nn, instruction );
            n = 0x0000;
            break;

        case ecf_nnn :
            x = address( instruction );
            y = 0x0000;
            n = 0x0000;
            break;

        default : break;
    }

    std::snprintf( buffer_str, line_size, "%04X | 0x%X 0x%03X 0x%03X 0x%03X |", PC, opcode, x, y, n );

    printf( "%s\n", buffer_str );
}

void chip8_cpu_manager_unit::update_timers( ) {
    timers.update( );
}

void chip8_cpu_manager_unit::consume( ) { 
    PC += 2; 
}

echip8_states chip8_cpu_manager_unit::execute(
    const uint16_t instruction,
    chip8_memory_manager_unit& mmu,
    chip8_screen_manager_unit& smu
) {
    consume( );

    return opcodes.execute( instruction, chip8_self, mmu, smu );
}

void chip8_cpu_manager_unit::dump_timers(  ) const {
    timers.dump( );
}

void chip8_cpu_manager_unit::dump_opcodes( ) const {
    opcodes.dump( );
}

void chip8_cpu_manager_unit::dump_options( ) const {
    options.dump( );
}

void chip8_cpu_manager_unit::dump_state( ) const {
    printf( "PC 0x%04X\nI 0x0%04X\n", PC, I );
}

void chip8_cpu_manager_unit::dump( chip8_memory_manager_unit& mmu ) const {
    printf( "> CPU :\n" );
    
    dump_options( );
    dump_state( );
    dump_timers( );
    mmu.dump_cpu( );
    dump_opcodes( );
}

////////////////////////////////////////////////////////////////////////////////////////////
//		===	PUBLIC GET ===
////////////////////////////////////////////////////////////////////////////////////////////
chip8_cpu_option_manager& chip8_cpu_manager_unit::get_options( ) {
    return options;
}

bool chip8_cpu_manager_unit::get_option( const echip8_cpu_options option ) const {
    return options.get( option );
}

chip8_string chip8_cpu_manager_unit::get_opcode_name( const uint8_t opcode ) const {
    return opcodes.get_name( opcode );
}

uint8_t chip8_cpu_manager_unit::get_delay_timer( ) const {
    return timers.get_delay( );
}

uint8_t chip8_cpu_manager_unit::get_sound_timer( ) const {
    return timers.get_sound( );
}

uint8_t chip8_cpu_manager_unit::nibble(
    const echip8_nibbles target,
    const uint16_t instruction
) const {
    switch ( target ) {
        case ecn_x  : return ( instruction & ecm_x  ) >> 8;
        case ecn_y  : return ( instruction & ecm_y  ) >> 4;
        case ecn_n  : return ( instruction & ecm_n  );
        case ecn_nn : return ( instruction & ecm_nn );
        
        default : break;
    }

    return 0x00;
}

uint16_t chip8_cpu_manager_unit::address(
    const uint16_t instruction
) const {
    return ( instruction & ecm_nnn );
}

uint8_t chip8_cpu_manager_unit::try_map_key( const uint8_t key ) const {
    auto mapped_key = key;

    if ( std::isdigit( key ) )
        mapped_key = key - '0';
    else if ( std::isxdigit( key ) )
        mapped_key = ( std::tolower( key ) - 'a' ) + 0xA;

    return mapped_key;
}

bool chip8_cpu_manager_unit::validate_key( const uint8_t key ) const {
    return key < eci_key_count;
}

std::tuple<bool, uint8_t> chip8_cpu_manager_unit::get_key(
    const uint16_t instruction,
    const chip8_memory_manager_unit& mmu
) const {
    auto key_valid = false;
    auto key       = uint8_t( eci_key_undefined );

    if ( get_key_callback ) {
        const auto key_value = std::invoke( get_key_callback, instruction, chip8_self, mmu );
        
        if ( key_valid = validate_key( key_value ) )
            key = key_value;
    }

    return { key_valid, key };
}
