#include "camera.h"
#include "shader.h"
#include "model.h"
#include "../dungeon/dungeon_builder.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <SDL.h>
#include <GL/glew.h>

#include <iostream>
#include <algorithm>
#include <vector>
#include <memory>
#include <filesystem>
#include <string>


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

    Shader default_shader;

    GLuint shader_program;

    Camera player_camera;

    DungeonBuilder dungeon_builder;
    std::vector<DungeonTile> dungeon_tiles;

    int screen_width;
    int screen_height;

    int quit_flag;
};