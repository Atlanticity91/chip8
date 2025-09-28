#include "chip8.h"

////////////////////////////////////////////////////////////////////////////////////////////
//		===	PUBLIC ===
////////////////////////////////////////////////////////////////////////////////////////////
chip8_stack_mananger::chip8_stack_mananger( )
    : stack{ }
{ }

bool chip8_stack_mananger::push(
    const uint16_t address,
    const bool is_unlimited
) {
    if ( !is_unlimited && stack.size( ) == StackSize )
        return false;

    stack.push( address );

    return true;
}

void chip8_stack_mananger::dump( ) const {
    auto local_stack = stack;
    auto stack_id    = local_stack.size( );

    printf( "> Stack %d :\n", stack_id );

    while ( stack_id-- > 0 ) {
        printf( "[ %2d ] 0x%4X\n", stack_id, local_stack.top( ) );
            
        local_stack.pop( );

        if ( stack_id == 17 )
            printf( ">> Default chip 8 16 slot stack :\n" );
    }
}

////////////////////////////////////////////////////////////////////////////////////////////
//		===	PUBLIC GET ===
////////////////////////////////////////////////////////////////////////////////////////////
std::tuple<uint16_t, bool> chip8_stack_mananger::pop( ) {
    auto address = eca_null;

    if ( stack.size( ) > 0 ) {
        const auto address = stack.top( );

        stack.pop( );
    }
        
    return std::make_tuple( address, true );
}
