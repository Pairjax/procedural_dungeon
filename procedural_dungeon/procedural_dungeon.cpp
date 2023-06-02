#include <iostream>
#include <algorithm>

#include <SDL.h>
#include <GL/glew.h>

#define SCREEN_WIDTH    800
#define SCREEN_HEIGHT   600

struct proc_dungeon_world {
    SDL_Window* window;
    SDL_Renderer* renderer;
};

proc_dungeon_world world = {};

int init_SDL() {
    // Initialize SDL
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cout << "SDL could not be initialized!" << std::endl
            << "SDL_Error: " << SDL_GetError() << std::endl;

        return 0;
    }

    // Create window
    world.window = SDL_CreateWindow("Procedural Dungeon",
        SDL_WINDOWPOS_UNDEFINED,
        SDL_WINDOWPOS_UNDEFINED,
        SCREEN_WIDTH, SCREEN_HEIGHT,
        SDL_WINDOW_SHOWN);

    if (!world.window) {
        std::cout << "Window could not be created!" << std::endl
            << "SDL_Error: " << SDL_GetError() << std::endl;

        SDL_Quit();
        return 0;
    }

    // Create renderer
    world.renderer = SDL_CreateRenderer(world.window, -1, SDL_RENDERER_ACCELERATED);
    if (!world.renderer) {
        std::cout << "Renderer could not be created!" << std::endl
            << "SDL_Error: " << SDL_GetError() << std::endl;

        SDL_Quit();
        return 0;
    }

    return 1;
}

int terminate_SDL() {
    SDL_DestroyRenderer(world.renderer);
    SDL_DestroyWindow(world.window);

    SDL_Quit();

    // Leaving this in case of future cases where an error could arise.
    return 1;
}

int init_gamestate() {
    return 0;
}

int terminate_gamestate() {
    return 0;
}

int main(int argc, char* argv[]) {
    // Unused argc, argv
    (void)argc;
    (void)argv;
    
    if (!init_SDL()) {
        return 0;
    }

    // Declare rect of square
    SDL_Rect squareRect;

    // Square dimensions: Half of the min(SCREEN_WIDTH, SCREEN_HEIGHT)
    squareRect.w = std::min(SCREEN_WIDTH, SCREEN_HEIGHT) / 2;
    squareRect.h = std::min(SCREEN_WIDTH, SCREEN_HEIGHT) / 2;

    // Square position: In the middle of the screen
    squareRect.x = SCREEN_WIDTH / 2 - squareRect.w / 2;
    squareRect.y = SCREEN_HEIGHT / 2 - squareRect.h / 2;
    
    // Event loop exit flag
    bool quit = false;

    // Event loop
    while (!quit)
    {
        SDL_Event e;

        // Wait indefinitely for the next available event
        SDL_WaitEvent(&e);

        // User requests quit
        if (e.type == SDL_QUIT)
        {
            quit = true;
        }

        // Initialize renderer color white for the background
        SDL_SetRenderDrawColor(world.renderer, 0xFF, 0xFF, 0xFF, 0xFF);

        // Clear screen
        SDL_RenderClear(world.renderer);

        // Set renderer color red to draw the square
        SDL_SetRenderDrawColor(world.renderer, 0xFF, 0x00, 0x00, 0xFF);

        // Draw filled square
        SDL_RenderFillRect(world.renderer, &squareRect);

        // Update screen
        SDL_RenderPresent(world.renderer);
    }

    terminate_SDL();

    return 0;
}