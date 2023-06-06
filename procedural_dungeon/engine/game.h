#include <SDL.h>
#include <GL/glew.h>

#include <iostream>
#include <algorithm>
#include <vector>
#include <memory>

class Game {
public:
    Game();

    SDL_Window* get_window();
    SDL_Renderer* get_renderer();

    int init(int screen_width, int screen_height);
    void quit();

    int load();
    int handle_input();
    int start();
    int update(unsigned int delta);
    int render();
    int clean();

private:
    SDL_Window* window;
    SDL_Renderer* renderer;
    SDL_GLContext context;

    GLuint shader_program;

    int quit_flag;
};