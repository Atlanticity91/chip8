#include "chip8.h"

int main( int argc, char** argv ) {
    if ( argc < 2 ) {
        printf( "> No ROM given for execution." );

        return -1;
    }

    for ( auto rom_id = 1; rom_id < argc; rom_id++ ) {
        const auto* rom_path = argv[ rom_id ];
        auto chip8_emulator  = chip8{ };
        auto exec_state      = chip8_emulator.execute( rom_path );

        chip8_emulator.print_exec_state( exec_state );

        if ( exec_state == ecs_eop )
            chip8_emulator.dump( ecdm_screen );
        else {
            chip8_emulator.dump( ecdm_all );

            printf( "> Execution of ROM %s failed.", rom_path );

            break;
        }
    }

    return 0;
}
