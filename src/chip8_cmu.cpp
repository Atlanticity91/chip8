#include "chip8.h"

////////////////////////////////////////////////////////////////////////////////////////////
//		===	PUBLIC ===
////////////////////////////////////////////////////////////////////////////////////////////
chip8_cpu_manager_unit::chip8_cpu_manager_unit(
    const bool legacy_mode,
    const bool enable_print
)
    : use_legacy{ legacy_mode },
    use_print{ enable_print },
    PC{ 0 },
    I{ 0 },
    delay_timer{ 60 },
    sound_timer{ 60 },
    opcode_names{ "unimplemented" }
{
    auto exec_unimplemented = [](
        const uint16_t,
        chip8_cpu_manager_unit&,
        chip8_memory_manager_unit&,
        chip8_screen_manager_unit&
    ) { 
        printf( "unimplemented\n" );
            
        return ecs_uop;
    };

    for ( auto& opcode : opcodes )
        opcode = exec_unimplemented;
}

void chip8_cpu_manager_unit::print_instruction( 
    const uint16_t instruction,
    const echip8_formats format
) {
    if ( !use_print )
        return;

    const auto line_size = 31;
    const auto opcode = uint8_t( instruction >> 12 );
    auto line = std::array<char, line_size>{ "0000 | 0x0 0x000 0x000 0x000 |" };
    auto* line_string = line.data( );
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

    std::snprintf( line_string, line_size, "%04X | 0x%X 0x%03X 0x%03X 0x%03X |", PC, opcode, x, y, n );

    printf( "%s\n", line_string );
}

void chip8_cpu_manager_unit::register_op(
    const uint8_t opcode,
    const char* opcode_name,
    chip8_opcode&& implementation
) {
    if ( opcode < uint16_t( opcodes.size( ) ) && implementation ) {
        opcodes[ opcode ] = std::move( implementation );
        opcode_names[ opcode ] = opcode_name ? opcode_name : "~unnamed";
    }
}

void chip8_cpu_manager_unit::update_timers( ) {
    if ( delay_timer > 0 )
        delay_timer -= 1;

    if ( sound_timer > 0 )
        sound_timer -= 1;
}

void chip8_cpu_manager_unit::consume( ) { 
    PC += 2; 
}

echip8_states chip8_cpu_manager_unit::execute(
    const uint16_t instruction,
    chip8_memory_manager_unit& mmu,
    chip8_screen_manager_unit& smu
) {
    const auto instruction_opcode = uint16_t( instruction >> 12 );

    consume( );

    return std::invoke( opcodes[ instruction_opcode ], instruction, *this, mmu, smu );
}

void chip8_cpu_manager_unit::dump( chip8_memory_manager_unit& mmu ) {
    mmu.dump_cpu( );

    printf( "> CPU :\nPC 0x%04X\nI 0x0%04X\nDelay Timer %3d\nSound Timer %3d\n", PC, I, delay_timer.load( ), sound_timer.load( ) );
    printf( "> Instruction :\n" );

    auto opcode = 0;

    for ( const auto name : opcode_names )
        printf( "[ 0x%02X ] %s\n", opcode++, name );
}

////////////////////////////////////////////////////////////////////////////////////////////
//		===	PUBLIC GET ===
////////////////////////////////////////////////////////////////////////////////////////////
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
