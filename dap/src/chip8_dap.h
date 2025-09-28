#pragma once

#include "chip8.h"

/**
 * Define all supported commands.
 **/
enum echip8_dap_command_types : uint8_t {
    ecd_command_init = 0,
    ecd_command_next,
    ecd_command_quit,
    ecd_command_count
};

struct echip8_dap_payload_request {
    int sequence = 0;
};

union echip8_dap_payload {
    echip8_dap_payload_request request;
};

struct echip8_dap_command {
    echip8_dap_command_types type;
    echip8_dap_payload payload;
};

/**
 * chip8_dap class
 * @note Define Debug Adapter (DAP) for chip8 emulator.
 **/
class chip8_dap final {

public:
    /**
     * Constructor
     **/
    chip8_dap( );

    void send_response( const echip8_dap_command& command );

    bool execute_command(
        const echip8_dap_command& command,
        chip8& chip8_instance
    );

public:
    /**
     * run function
     * @note Run the Debug Adapter.
     * @param argc : Target input argument count.
     * @param argv : Target input argument value.
     * @return Return execution state.
     **/
    static int run( int argc, char** argv );

public:
    echip8_dap_command receive_command( );

};
