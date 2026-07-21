// Make minimal modifications to this file
// You only really need to call your engine and initialize it
// with your assigned color
#include "chess.h"

void chess_engine(int color);

int main(int argc, char const *argv[])
{
    // Gets your randomly assigned color
    // -1 -> error
    // 1 -> White
    // 2 -> Black
    
    (void)argc;
    (void)argv;

    int color = get_color();
    if (color == -1)
        return -1;
    
    // Call your engine here to play as color
    chess_engine(color);

    return 0;
}
