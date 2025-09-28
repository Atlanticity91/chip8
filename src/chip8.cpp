#include "chip8_timer_manager.h"

////////////////////////////////////////////////////////////////////////////////////////////
//		===	PUBLIC ===
////////////////////////////////////////////////////////////////////////////////////////////
chip8::chip8( )
    : chip8{ false, true, true }
{ }

chip8::chip8( 
    const bool legacy_mode,
    const bool enable_print,
    const bool enable_stack_limit
)
    : mmu{ },
    smu{ },
    cpu{ legacy_mode, enable_print, enable_stack_limit },
    rom{ }
{
    reset_opcodes( );
    reset_get_key( );
}

void chip8::reset( ) {
    mmu.reset( );
    smu.clear( );
    cpu.reset( );
}

#define cregister_op( OP, NAME )\
    cpu.register_op( OP, #NAME, chip8_cpu_implementation::exec_##NAME )

void chip8::reset_opcodes( ) {
    cregister_op( 0x0, 0GGG_routine );
    cregister_op( 0x1, 1NNN_jump );
    cregister_op( 0x2, 2NNN_subroutines );
    cregister_op( 0x3, 3XNN_skip );
    cregister_op( 0x4, 4XNN_skip );
    cregister_op( 0x5, 5XY0_skip );
    cregister_op( 0x6, 6XNN_set );
    cregister_op( 0x7, 7XNN_add );
    cregister_op( 0x8, 8XYG_logic );
    cregister_op( 0x9, 9XY0_skip );
    cregister_op( 0xA, ANNN_set_i );
    cregister_op( 0xB, BNNN_jump );
    cregister_op( 0xC, CXNN_random );
    cregister_op( 0xD, DXYN_display );
    cregister_op( 0xE, EXGG_skip_if_key );
    cregister_op( 0xF, FXGG_iomanip ); 
}

void chip8::reset_get_key( ) {
    cpu.set_key_callback( chip8_cpu_implementation::exec_get_key_callback );
}

void chip8::override_opcode(
    const uint8_t opcode,
    chip8_string opcode_name,
    chip8_opcode&& implementation
) {
    cpu.register_op( opcode, opcode_name, std::move( implementation ) );
}

void chip8::override_key_callback( chip8_get_key&& callback ) {
    cpu.set_key_callback( std::move( callback ) );
}

void chip8::set_delay_timer( const uint8_t value ) {
    cpu.set_delay_timer( value );
}

void chip8::set_sound_timer( const uint8_t value ) {
    cpu.set_sound_timer( value );
}

void chip8::set_option(
    const echip8_cpu_options option,
    const bool value
) {
    cpu.set_option( option, value );
}

std::vector<chip8_string> chip8::parse_arguments(
    const int argc,
    char** argv
) {
    auto rom_list = std::vector<chip8_string>{ };

    for ( auto arg_id = 1; arg_id < argc; arg_id++ ) {
        const auto* argument = argv[ arg_id ];

        if ( argument[ 0 ] == '-' )
            parse_option( argument );
        else if ( std::filesystem::is_regular_file( argument ) )
            rom_list.emplace_back( argument );
    }

    return rom_list;
}

bool chip8::load_rom( chip8_string rom_path ) {
    return rom.load( mmu, rom_path );
}

echip8_states chip8::execute( const uint32_t instruction_per_second ) {
    if ( !rom.exist( ) )
        return ecs_nip;

    const auto* rom_path = rom.get_path( );
    const auto rom_size  = rom.get_size( );

    printf( "> Executing ROM : %s\n", rom_path );

    auto instruction_count = 0;
    auto timer_manager     = echip8_timer_manager{ cpu };
    auto cycle_start       = clock_t::now( );
    auto state             = ecs_run;

    reset( );

    while ( cpu.PC < rom_size && state == ecs_run ) {
        const auto instruction = rom.fetch( mmu, cpu.PC );

        state = cpu.execute( instruction, mmu, smu );

        if ( cpu.get_option( ecc_option_limit ) && instruction_per_second > 0 )
            try_wait( instruction_per_second, instruction_count, cycle_start );
    }

    timer_manager.terminate( );

    if ( state == ecs_run && rom_size <= cpu.PC )
        state = ecs_eop;

    return state;
}

echip8_states chip8::execute( 
    chip8_string rom_path,
    const uint32_t instruction_per_second
) {
    if ( !rom.load( mmu, rom_path ) )
        return ecs_iir;
    
    return execute( instruction_per_second );
}

void chip8::dump( const echip8_dump_modes mode ) {
    printf( "\n=== DUMP ===\n" );

    switch ( mode ) {
        case ecdm_screen  : smu.dump( );         break;
        case ecdm_memory  : mmu.dump( );         break;
        case ecdm_cpu     : cpu.dump( mmu );     break;
        case ecdm_timers  : cpu.dump_timers( );  break;
        case ecdm_opcodes : cpu.dump_opcodes( ); break;
        case ecdm_options : cpu.dump_options( ); break;
        case ecdm_state   : cpu.dump_state( );   break;
        case ecdm_font    : mmu.dump_font( );    break;
        case ecdm_rom     : rom.dump( mmu );     break;

        case ecdm_all :
            cpu.dump( mmu );
            mmu.dump( );
            smu.dump( );
            break;

        default : break;
    }
}

void chip8::print_exec_state( const echip8_states exec_state ) {
    auto* state_string = "Undefined";

    switch ( exec_state ) {
        case ecs_run : state_string = "Running";              break;
        case ecs_eop : state_string = "End Of Program";       break;
        case ecs_nip : state_string = "No Input ROM";         break;
        case ecs_iir : state_string = "Invalid Input Rom";    break;
        case ecs_uop : state_string = "Unimplemented OPcode"; break;
        case ecs_sgf : state_string = "Seg Fault";            break;
        case ecs_iik : state_string = "Invalid Input Key";    break;
        default : break;
    }

    printf( "> Execution State : %s\n", state_string );
}

////////////////////////////////////////////////////////////////////////////////////////////
//		===	PRIVATE ===
////////////////////////////////////////////////////////////////////////////////////////////
void chip8::parse_option( chip8_string argument ) {
    switch ( argument[ 1 ] ) {
        case 'i' :
        case 'I' :
            cpu.set_option( ecc_option_limit, argument[ 2 ] == '1' );
            break;

        case 'l' :
        case 'L' :
            cpu.set_option( ecc_option_legacy, true );
            break;

        case 'p' :
        case 'P' :
            cpu.set_option( ecc_option_print, argument[ 2 ] == '1' );
            break;

        case 's' :
        case 'S' :
            cpu.set_option( ecc_option_stack, argument[ 2 ] == '1' );
            break;

        default: break;
    }
}

void chip8::try_wait(
    const uint32_t instruction_per_second,
    int& instruction_counter,
    clock_t::time_point& cycle_start
) {
    constexpr auto duration = std::chrono::seconds( 1 );

    if ( ++instruction_counter == instruction_per_second ) {
        auto stop = clock_t::now( );
        auto elapsed = stop - cycle_start;

        if ( elapsed < duration )
            std::this_thread::sleep_for( duration - elapsed );

        instruction_counter = 0;
        cycle_start = stop;
    }
}

////////////////////////////////////////////////////////////////////////////////////////////
//		===	PUBLIC GET ===
////////////////////////////////////////////////////////////////////////////////////////////
chip8_memory_manager_unit& chip8::get_mmu( ) {
    return mmu;
}

chip8_screen_manager_unit& chip8::get_smu( ) {
    return smu;
}

chip8_cpu_manager_unit& chip8::get_cpu( ) {
    return cpu;
} 

chip8_rom_manager_unit& chip8::get_rom( ) {
    return rom;
}

const uint8_t* chip8::get_screen_buffer( ) const {
    return smu.get_screen_buffer( );
}
