#include "chip8.h"

int main( int argc, char** argv ) {
    if ( argc < 2 ) {
        printf( "> No ROM given for execution." );

        return -1;
    }

    auto chip8_emulator = chip8{ };
    auto rom_list       = chip8_emulator.parse_arguments( argc, argv );

    for ( const auto* rom : rom_list ) {
        auto exec_state = chip8_emulator.execute( rom );

        chip8_emulator.print_exec_state( exec_state );

        if ( exec_state == ecs_eop )
            chip8_emulator.dump( ecdm_screen );
        else {
            chip8_emulator.dump( ecdm_all );

            printf( "> Execution of ROM %s failed.", rom );

            break;
        }
    }

    return 0;
}
