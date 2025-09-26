#include "chip8.h"

#define cregister_op( OP, NAME )\
    cpu.register_op( OP, #NAME, chip8_cpu_implementation::exec_##NAME )

////////////////////////////////////////////////////////////////////////////////////////////
//		===	PUBLIC ===
////////////////////////////////////////////////////////////////////////////////////////////
chip8::chip8( 
    const bool legacy_mode,
    const bool enable_print
)
    : mmu{ },
    smu{ },
    cpu{ legacy_mode, enable_print }
{ 
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

void chip8::override(
    const uint8_t opcode,
    const char* opcode_name,
    chip8_opcode&& implementation
) {
    cpu.register_op( opcode, opcode_name, std::move( implementation ) );
}

class echip8_timer_manager final {

    using clock_t = std::chrono::steady_clock;

private:
    std::atomic<bool> is_running;
    std::thread thread;

public:
    echip8_timer_manager( chip8_cpu_manager_unit& cpu ) 
        : is_running{ true }
    {
        auto exec_lambda = [&]() -> void {
            constexpr auto duration = std::chrono::milliseconds( 16 );
            auto next_tick = clock_t::now( ) + duration;

            while ( is_running ) {
                std::this_thread::sleep_until( next_tick );

                next_tick = std::max( next_tick + duration, clock_t::now( ) );

                cpu.update_timers( );
            }
        };

        thread = std::move( std::thread( exec_lambda ) );
    };

    void terminate( ) {
        if ( thread.joinable( ) ) {
            is_running = false;

            thread.join( );
        }
    };

};

echip8_states chip8::execute( 
    const char* rom_path,
    const uint32_t instruction_per_second
) {
    if ( !mmu.load_rom( rom_path ) )
        return ecs_iir;

    const auto rom_size = mmu.get_rom_size( );

    printf( "> Executing ROM : %s\n", rom_path );

    auto instruction_count = 0;
    auto timer_manager     = echip8_timer_manager{ cpu };
    auto cycle_start       = clock_t::now( );
    auto state             = ecs_run;

    while ( cpu.PC < rom_size && state == ecs_run ) {
        const auto instruction = mmu.fetch( cpu.PC );

        state = cpu.execute( instruction, mmu, smu );

        try_wait( instruction_per_second, instruction_count, cycle_start );
    }

    timer_manager.terminate( );

    if ( state == ecs_run && rom_size <= cpu.PC )
        state = ecs_eop;

    return state;
}

/*
    std::atomic<bool> is_running = true;
    std::thread timer_trick( [&]() -> void {
        constexpr auto duration = std::chrono::milliseconds( 16 );

        while ( is_running ) {
            std::this_thread::sleep_for( duration );

            cpu.update_timers( );
        }
    } );
    */

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
        cycle_start         = stop;
    }
}

void chip8::dump( const echip8_dump_modes mode ) {
    printf( "\n=== DUMP ===\n" );

    switch ( mode ) {
        case ecdm_screen : smu.dump( );     break;
        case ecdm_memory : mmu.dump( );     break;
        case ecdm_cpu    : cpu.dump( mmu ); break;

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
        case ecs_iir : state_string = "Invalid Input Rom";    break;
        case ecs_uop : state_string = "Unimplemented OPcode"; break;
        case ecs_sgf : state_string = "Seg Fault";            break;
        case ecs_iik : state_string = "Invalid Input Key";    break;
        default : break;
    }

    printf( "> Execution State : %s\n", state_string );
}

////////////////////////////////////////////////////////////////////////////////////////////
//		===	PUBLIC GET ===
////////////////////////////////////////////////////////////////////////////////////////////
const uint8_t* chip8::get_screen_buffer( ) const {
    return smu.get_screen_buffer( );
}
