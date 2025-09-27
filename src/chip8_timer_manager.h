#include "chip8.h"

/**
 * echip8_timer_manager class
 * @note Manage thread for timer 60Hz management.
 **/
class echip8_timer_manager final {

    using clock_t = std::chrono::steady_clock;

private:
    std::atomic<bool> is_running;
    std::thread thread;

public:
    /**
     * Constructor
     * @param cpu : Reference to current cpu manager unit.
     **/
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

    /**
     * terminate method
     * @note Terminate the timer thread.
     **/
    void terminate( ) {
        if ( thread.joinable( ) ) {
            is_running = false;

            thread.join( );
        }
    };

};
