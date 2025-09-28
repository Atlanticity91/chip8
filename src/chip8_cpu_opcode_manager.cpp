#include "chip8.h"

////////////////////////////////////////////////////////////////////////////////////////////
//		===	PUBLIC ===
////////////////////////////////////////////////////////////////////////////////////////////
chip8_cpu_opcode_manager::chip8_cpu_opcode_manager( )
{
    initialize( );
}

void chip8_cpu_opcode_manager::set(
    const uint8_t opcode,
    chip8_string opcode_name,
    chip8_opcode&& implementation
) {
    if ( opcode > Count || !implementation )
        return;

    opcodes[ opcode ].exec = std::move( implementation );
    opcodes[ opcode ].name = opcode_name ? opcode_name : Unnamed;
}

echip8_states chip8_cpu_opcode_manager::execute(
    const uint16_t instruction,
    chip8_cpu_manager_unit& cpu,
    chip8_memory_manager_unit& mmu,
    chip8_screen_manager_unit& smu
) {
    const auto instruction_opcode = uint16_t( instruction >> 12 );

    return std::invoke( opcodes[ instruction_opcode ].exec, instruction, cpu, mmu, smu );
}

void chip8_cpu_opcode_manager::dump( ) const {
    printf( "> Instruction :\n" );

    auto opcode_id = 0;

    for ( const auto opcode : opcodes )
        printf( "[ 0x%02X ] %s\n", opcode_id++, opcode.name );
}

////////////////////////////////////////////////////////////////////////////////////////////
//		===	PRIVATE ===
////////////////////////////////////////////////////////////////////////////////////////////
void chip8_cpu_opcode_manager::initialize( ) {
    auto exec_unimplemented = [](
        const uint16_t,
        chip8_cpu_manager_unit& cpu,
        chip8_memory_manager_unit&,
        chip8_screen_manager_unit&
     ) {
        if ( cpu.get_option( ecc_option_print ) )
            printf( "unimplemented\n" );
                
        return ecs_uop;
    };

    for ( auto& opcode : opcodes )
        opcode.exec = exec_unimplemented;
}

////////////////////////////////////////////////////////////////////////////////////////////
//		===	PUBLIC GET ===
////////////////////////////////////////////////////////////////////////////////////////////
chip8_string chip8_cpu_opcode_manager::get_name( const uint8_t opcode ) const {
    if ( opcode < Count )
        return opcodes[ opcode ].name;

    return Unnamed;
}
