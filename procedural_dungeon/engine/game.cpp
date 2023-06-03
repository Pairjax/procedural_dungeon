#include "game.h"

Game::Game() {

}

int Game::init(int screen_width, int screen_height) {
    /*// Initialize SDL
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cout << "SDL could not be initialized!" << std::endl
            << "SDL_Error: " << SDL_GetError() << std::endl;

        return 0;
    }

    // Create window
    window = SDL_CreateWindow("Procedural Dungeon",
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

    return 1;*/
}

void Game::terminate() {

}

SDL_Window* Game::get_window() {
	return window;
}

SDL_Renderer* Game::get_renderer() {
	return renderer;
}