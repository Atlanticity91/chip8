#include "chip8.h"

////////////////////////////////////////////////////////////////////////////////////////////
//		===	PUBLIC ===
////////////////////////////////////////////////////////////////////////////////////////////
namespace chip8_cpu_implementation {

    echip8_states exec_0GGG_routine(
        const uint16_t instruction,
        chip8_cpu_manager_unit& cpu,
        chip8_memory_manager_unit& mmu,
        chip8_screen_manager_unit& smu
    ) {
        cpu.print_instruction( instruction, ecf_nnn );

        if ( instruction == 0x00E0 ) {
            smu.clear( );

            return ecs_run;
        } else if ( instruction != 0x00EE ) {
            const auto tuple = mmu.pop( );

            cpu.PC = std::get<uint16_t>( tuple );

            return std::get<bool>( tuple ) ? ecs_run : ecs_sgf;
        }

        return ecs_uop;
    }
    
    echip8_states exec_1NNN_jump(
        const uint16_t instruction,
        chip8_cpu_manager_unit& cpu,
        chip8_memory_manager_unit& mmu,
        chip8_screen_manager_unit& smu
    ) {
        cpu.print_instruction( instruction, ecf_nnn );

        cpu.PC = cpu.address( instruction );

        return ecs_run;
    }
    
    echip8_states exec_2NNN_subroutines(
        const uint16_t instruction,
        chip8_cpu_manager_unit& cpu,
        chip8_memory_manager_unit& mmu,
        chip8_screen_manager_unit& smu
    ) {
        cpu.print_instruction( instruction, ecf_nnn );

        const auto stack_option = cpu.get_option( ecc_option_stack );
        const auto nnn          = cpu.address( instruction );

        if ( mmu.push( cpu.PC, stack_option ) ) {
            cpu.PC = nnn;

            return ecs_run;
        }

        return ecs_sgf;
    }
    
    echip8_states exec_3XNN_skip(
        const uint16_t instruction,
        chip8_cpu_manager_unit& cpu,
        chip8_memory_manager_unit& mmu,
        chip8_screen_manager_unit& smu
    ) {
        cpu.print_instruction( instruction, ecf_xnn );

        const auto x = cpu.nibble( ecn_x, instruction );

        if ( mmu.v( x ) == cpu.nibble( ecn_nn, instruction ) )
            cpu.consume( );

        return ecs_run;
    }
    
    echip8_states exec_4XNN_skip(
        const uint16_t instruction,
        chip8_cpu_manager_unit& cpu,
        chip8_memory_manager_unit& mmu,
        chip8_screen_manager_unit& smu
    ) {
        cpu.print_instruction( instruction, ecf_xnn );

        const auto x = cpu.nibble( ecn_x, instruction );

        if ( mmu.v( x ) != cpu.nibble( ecn_nn, instruction ) )
            cpu.consume( );

        return ecs_run;
    }
    
    echip8_states exec_5XY0_skip(
        const uint16_t instruction,
        chip8_cpu_manager_unit& cpu,
        chip8_memory_manager_unit& mmu,
        chip8_screen_manager_unit& smu
    ) {
        cpu.print_instruction( instruction, ecf_xyn );

        const auto x = cpu.nibble( ecn_x, instruction );
        const auto y = cpu.nibble( ecn_y, instruction );

        if ( mmu.v( x ) == mmu.v( y ) )
            cpu.consume( );

        return ecs_run;
    }
    
    echip8_states exec_6XNN_set(
        const uint16_t instruction,
        chip8_cpu_manager_unit& cpu,
        chip8_memory_manager_unit& mmu,
        chip8_screen_manager_unit& smu
    ) {
        cpu.print_instruction( instruction, ecf_xnn );

        const auto x = cpu.nibble( ecn_x, instruction );
        
        mmu.v( x ) = cpu.nibble( ecn_nn, instruction );

        return ecs_run;
    }
    
    echip8_states exec_7XNN_add(
        const uint16_t instruction,
        chip8_cpu_manager_unit& cpu,
        chip8_memory_manager_unit& mmu,
        chip8_screen_manager_unit& smu
    ) {
        cpu.print_instruction( instruction, ecf_xnn );

        const auto x = cpu.nibble( ecn_x, instruction );
        
        mmu.v( x ) += cpu.nibble( ecn_nn, instruction );
        
        return ecs_run;
    }
    
    echip8_states exec_8XYG_logic(
        const uint16_t instruction,
        chip8_cpu_manager_unit& cpu,
        chip8_memory_manager_unit& mmu,
        chip8_screen_manager_unit& smu
    ) {
        cpu.print_instruction( instruction, ecf_xyn );

        const auto x = cpu.nibble( ecn_x, instruction );
        const auto y = cpu.nibble( ecn_y, instruction );
        const auto n = cpu.nibble( ecn_n, instruction );

        switch ( n ) {
            case 0x0 : mmu.v( x )  = mmu.v( y ); break; // Set
            case 0x1 : mmu.v( x ) |= mmu.v( y ); break; // Binary OR
            case 0x2 : mmu.v( x ) &= mmu.v( y ); break; // Binary AND
            case 0x3 : mmu.v( x ) ^= mmu.v( y ); break; // Logical XOR

            // Add
            case 0x4 :
                mmu.v( 0xF ) = 1 * ( 255 < uint16_t( mmu.v( x ) + mmu.v( y ) ) );
                mmu.v( x ) += mmu.v( y );

                break;

            // Subtract vx - vy
            case 0x5 : 
                mmu.v( 0xF ) = 1 * ( mmu.v( y ) < mmu.v( x ) );
                mmu.v( x ) -= mmu.v( y ); 
                break;

            // Shift 1 >>
            case 0x6 :
                if ( cpu.get_option( ecc_option_legacy ) )
                    mmu.v( x ) = mmu.v( y );

                mmu.v( 0xF ) = mmu.v( x ) & 0x01;
                mmu.v( x ) >>= 1; 
                break;

            // Subtract vy - vx
            case 0x7 :
                mmu.v( 0xF ) = 1 * ( mmu.v( x ) < mmu.v( y ) );
                mmu.v( x )  = mmu.v( y ) - mmu.v( x ); 
                break;

            // Shift 1 << 
            case 0xE :
                if ( cpu.get_option( ecc_option_legacy ) )
                    mmu.v( x ) = mmu.v( y );

                mmu.v( 0xF ) = ( mmu.v( x ) & 0x80 ) >> 7;
                mmu.v( x ) <<= 1; 
                break;

            default : return ecs_uop;
        }
        
        return ecs_run;
    }
    
    echip8_states exec_9XY0_skip(
        const uint16_t instruction,
        chip8_cpu_manager_unit& cpu,
        chip8_memory_manager_unit& mmu,
        chip8_screen_manager_unit& smu
    ) {
        cpu.print_instruction( instruction, ecf_xyn );

        const auto x = cpu.nibble( ecn_x, instruction );
        const auto y = cpu.nibble( ecn_y, instruction );

        if ( mmu.v( x ) != mmu.v( y ) )
            cpu.consume( );

        return ecs_run;
    }
     
    echip8_states exec_ANNN_set_i(
        const uint16_t instruction,
        chip8_cpu_manager_unit& cpu,
        chip8_memory_manager_unit& mmu,
        chip8_screen_manager_unit& smu
    ) {
        cpu.print_instruction( instruction, ecf_nnn );

        cpu.I = cpu.address( instruction );
        
        return ecs_run;
    }

    echip8_states exec_BNNN_jump(
        const uint16_t instruction,
        chip8_cpu_manager_unit& cpu,
        chip8_memory_manager_unit& mmu,
        chip8_screen_manager_unit& smu
    ) {
        cpu.print_instruction( instruction, ecf_nnn );

        const auto nnn = cpu.address( instruction );

        cpu.PC = nnn + mmu.v( 0 );

        return ecs_run;
    }
    
    echip8_states exec_CXNN_random(
        const uint16_t instruction,
        chip8_cpu_manager_unit& cpu,
        chip8_memory_manager_unit& mmu,
        chip8_screen_manager_unit& smu
    ) {
        cpu.print_instruction( instruction, ecf_xnn );

        const auto x = cpu.nibble( ecn_x, instruction );

        mmu.v( x ) = uint8_t( rand( ) ) & cpu.nibble( ecn_nn, instruction );

        return ecs_run;
    }
    
    echip8_states exec_DXYN_display(
        const uint16_t instruction,
        chip8_cpu_manager_unit& cpu,
        chip8_memory_manager_unit& mmu,
        chip8_screen_manager_unit& smu
    ) {
        cpu.print_instruction( instruction, ecf_xyn );

        const auto x  = cpu.nibble( ecn_x, instruction );
        const auto y  = cpu.nibble( ecn_y, instruction );
        const auto n  = cpu.nibble( ecn_n, instruction );
        const auto vx = mmu.v( x );
        const auto vy = mmu.v( y );

        smu.display( mmu, cpu, { vx, vy, n } );

        return ecs_run;
    }
    
    echip8_states exec_EXGG_skip_if_key(
        const uint16_t instruction,
        chip8_cpu_manager_unit& cpu,
        chip8_memory_manager_unit& mmu,
        chip8_screen_manager_unit& smu
    ) {
        cpu.print_instruction( instruction, ecf_xnn );

        const auto x  = cpu.nibble( ecn_x , instruction );
        const auto nn = cpu.nibble( ecn_nn, instruction );

        if ( ( nn == 0x9E && mmu.v( x ) ) || ( nn == 0xA1 && !mmu.v( x ) ) )
            cpu.consume( );

        return ecs_run;
    }

    echip8_states exec_get_key(
        const uint16_t instruction,
        chip8_cpu_manager_unit& cpu,
        chip8_memory_manager_unit& mmu,
        chip8_screen_manager_unit& smu
    ) {
        const auto [ is_valid, key ] = cpu.get_key( instruction, mmu );
        
        if ( is_valid ) {
            const auto x = cpu.nibble( ecn_x, instruction );

            mmu.v( x ) = key;

            return ecs_run;
        }

        return ecs_iik;
    };
    
    echip8_states exec_FXGG_iomanip(
        const uint16_t instruction,
        chip8_cpu_manager_unit& cpu,
        chip8_memory_manager_unit& mmu,
        chip8_screen_manager_unit& smu
    ) {
        cpu.print_instruction( instruction, ecf_xnn );

        const auto x  = cpu.nibble( ecn_x , instruction );
        const auto nn = cpu.nibble( ecn_nn, instruction );

        switch ( nn ) {
            case 0x07 : mmu.v( x ) = cpu.get_delay_timer( ); break;

            // Get key
            case 0x0A : return exec_get_key( instruction, cpu, mmu, smu );
            
            // Set delay timer
            case 0x15 : 
                {
                    const auto vx = mmu.v( x );

                    cpu.set_delay_timer( vx ); 
                }
                break;

            // Set sound timer
            case 0x18 : 
                { 
                    const auto vx = mmu.v( x );

                    cpu.set_sound_timer( vx );
                }
                break;

            case 0x1E : cpu.I += mmu.v( x ); break; // Add to index
            case 0x29 : cpu.I  = mmu.v( x ); break; // Font character

            //  Binary-coded decimal conversion
            case 0x33 :
                mmu.write( cpu.I    , mmu.v( x ) / 100        );
                mmu.write( cpu.I + 1, ( mmu.v( x ) / 10 ) %10 );
                mmu.write( cpu.I + 2, mmu.v( x ) % 10         );
                break;
            
            //Store memory
            case 0x55 :
                for ( auto i = 0; i < (x+1); i++ )
                    mmu.write( cpu.I + i, mmu.v( i ) );
                break;

            // Load memory
            case 0x65 :
                for ( auto i = 0; i < (x+1); i++ )
                    mmu.v( i ) = mmu.read( cpu.I + i );
                break;

            default : break;
        }

        return ecs_run;
    }

    uint8_t exec_get_key_callback(
        const uint16_t instruction,
        const chip8_cpu_manager_unit& cpu,
        const chip8_memory_manager_unit& mmu
    ) {
        printf( "Enter a character present in (0-9,a-f) : " );

        return uint8_t( std::getc( stdin ) );
    }

    uint8_t exec_get_key_random(
        const uint16_t instruction,
        const chip8_cpu_manager_unit& cpu,
        const chip8_memory_manager_unit& mmu
    ) {
        return rand( ) % eci_key_count;
    }

};
