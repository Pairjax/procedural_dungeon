#include <iostream>
#include <algorithm>

#include "engine/game.h"

#define SCREEN_WIDTH    800
#define SCREEN_HEIGHT   600

int main(int argc, char* argv[]) {
    // Unused argc, argv
    (void)argc;
    (void)argv;

    Game game = Game();
    
    if (!game.init(SCREEN_WIDTH, SCREEN_HEIGHT)) {
        return 0;
    }

    game.start();

    return 0;
}