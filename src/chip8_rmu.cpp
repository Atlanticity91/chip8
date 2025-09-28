#include "chip8.h"

////////////////////////////////////////////////////////////////////////////////////////////
//		===	PUBLIC ===
////////////////////////////////////////////////////////////////////////////////////////////
chip8_rom_manager_unit::chip8_rom_manager_unit( )
    : size{ 0 },
    path{ "" }
{ }

bool chip8_rom_manager_unit::load( 
    chip8_memory_manager_unit& mmu,
    chip8_string rom_path
) {
    if ( std::filesystem::is_regular_file( rom_path ) ) {
        size = uint16_t( std::filesystem::file_size( rom_path ) );

        if ( size > 0 ) {
            auto* rom_memory = (char*)mmu.get_rom_memory( );
            auto rom_file    = std::ifstream( rom_path, std::ios::binary );

            rom_file.read( rom_memory, size );
        }
    }

    return size > 0;
}

void chip8_rom_manager_unit::dump( const chip8_memory_manager_unit& mmu ) const {
    mmu.dump_rom( size );
}

////////////////////////////////////////////////////////////////////////////////////////////
//		===	PUBLIC GET ===
////////////////////////////////////////////////////////////////////////////////////////////
bool chip8_rom_manager_unit::exist( ) const {
    return size > 0;
}

std::tuple<uint8_t*, uint8_t*> chip8_rom_manager_unit::get( 
    const chip8_memory_manager_unit& mmu
) const {
    auto* rom_memory = mmu.get_rom_memory( );

    return { rom_memory, rom_memory + size };
}

uint16_t chip8_rom_manager_unit::get_size( ) const {
    return size;
}

chip8_string chip8_rom_manager_unit::get_path( ) const {
    return exist( ) ? path : "";
}

uint16_t chip8_rom_manager_unit::fetch(
    chip8_memory_manager_unit& mmu,
    const uint16_t cpu_pc
) const {
    const auto* rom_memory = (char*)mmu.get_rom_memory( );

    return uint16_t( ( rom_memory[ cpu_pc ] << 8 ) | rom_memory[ cpu_pc + 1 ] );
}
