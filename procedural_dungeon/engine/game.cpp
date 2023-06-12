#include "game.h"

Game::Game() {
    quit_flag = false;
}

SDL_Window* Game::get_window() {
    return window;
}

SDL_Renderer* Game::get_renderer() {
    return renderer;
}

int Game::init(int screen_width, int screen_height) {
    this->screen_width = screen_width;
    this->screen_height = screen_height;

    // Initialize SDL
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cout << "SDL could not be initialized!" << std::endl
            << "SDL_Error: " << SDL_GetError() << std::endl;

        return 0;
    }

    // Create window
    window = SDL_CreateWindow("Procedural Dungeon",
        SDL_WINDOWPOS_UNDEFINED,
        SDL_WINDOWPOS_UNDEFINED,
        screen_width, screen_height,
        SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);

    if (!window) {
        std::cout << "Window could not be created!" << std::endl
            << "SDL_Error: " << SDL_GetError() << std::endl;
         
        SDL_Quit();
        return 0;
    }

    // Create renderer
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer) {
        std::cout << "Renderer could not be created!" << std::endl
            << "SDL_Error: " << SDL_GetError() << std::endl;

        SDL_Quit();
        return 0;
    }

    // Context Setup
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);

    context = SDL_GL_CreateContext(window);
    if (!context) {
        std::cout << "Context could not be created!" << std::endl
            << "SDL_Error: " << SDL_GetError() << std::endl;

        SDL_Quit();
        return 0;
    }

    // GLEW Setup
    GLenum glew_error = glewInit();
    if (glew_error != GLEW_OK) {
        std::cout << "GLEW could not be initialized!" << std::endl
            << glewGetErrorString(glew_error) << std::endl;

        SDL_Quit();
        return 0;
    }

    glEnable(GL_BLEND);
    glEnable(GL_DEPTH_TEST);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    stbi_set_flip_vertically_on_load(true);

    default_shader = Shader("shaders/shader.vs", "shaders/shader.fs");

    // Dungeon Tiles
    dungeon_builder = DungeonBuilder();
    dungeon_builder.add_tile(
        DungeonTile(
            Model("resources/dungeon_tiles/dungeon_tile_big_l.obj"),
            { glm::vec2(0,0), glm::vec2(0,1), glm::vec2(1,1), glm::vec2(1,2)},
            {
                {std::pair(0,0), {WEST, SOUTH}},
                {std::pair(0,1), {NORTH}},
                {std::pair(1,1), {}},
                {std::pair(1,2), {EAST}}
            }
        )
    );
    dungeon_builder.add_tile(
        DungeonTile(
            Model("resources/dungeon_tiles/dungeon_tile_plus.obj"),
            { glm::vec2(0,0) },
            {
                {std::pair(0,0), {NORTH, EAST, SOUTH, WEST}}
            }
        )
    );
    dungeon_builder.add_tile(
        DungeonTile(
            Model("resources/dungeon_tiles/dungeon_tile_t.obj"),
            { glm::vec2(0,0), glm::vec2(0,1), glm::vec2(1,1), glm::vec2(-1, 1)},
            {
                {std::pair(0,0), {SOUTH}},
                {std::pair(0,1), {}},
                {std::pair(1,1), {EAST}},
                {std::pair(-1,1), {NORTH}},
            }
        )
    );
    dungeon_builder.add_tile(
        DungeonTile(
            Model("resources/dungeon_tiles/dungeon_tile_small_l.obj"),
            { glm::vec2(0,0), glm::vec2(0,1), glm::vec2(1,1) },
            {
                {std::pair(0,0), {WEST, SOUTH}},
                {std::pair(0,1), {NORTH}},
                {std::pair(1,1), {EAST}},
            }
        )
    );
    dungeon_builder.add_tile(
        DungeonTile(
            Model("resources/dungeon_tiles/dungeon_tile_horizontal_x.obj"),
            { glm::vec2(0,0), glm::vec2(0,1) },
            {
                {std::pair(0,0), {WEST, SOUTH}},
                {std::pair(0,1), {NORTH, EAST}},
            }
            )
    );

    dungeon_tiles = dungeon_builder.generate_dungeon();

    glViewport(0, 0, screen_width, screen_height);

    return 1;
}

void Game::quit() {
    SDL_GL_DeleteContext(context);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);

    SDL_Quit();
}

int Game::start() {
    SDL_GetWindowSize(window, &screen_width, &screen_height);
    GLdouble delta_time = 0.0f;
    Uint64 last_frame = 0L;
    Uint64 current_frame = SDL_GetPerformanceCounter();

    // Game pipeline loop
    while (!quit_flag) {
        last_frame = current_frame;
        delta_time = ((current_frame - last_frame) * 1000) / (GLdouble)SDL_GetPerformanceCounter();

        Game::update(delta_time);
        Game::clean();
        Game::render();
    }

    Game::quit();

    return 1;
}

int Game::update(unsigned int delta) {
    SDL_Event e;

    // Wait indefinitely for the next available event
    SDL_WaitEvent(&e);

    // User requests quit
    if (e.type == SDL_QUIT) {
        quit_flag = true;
    }

    return 1;
}

int Game::render() {
    glUseProgram(shader_program);
    // glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, 3);
    glBindVertexArray(0);

    default_shader.use();
    glm::mat4 projection = glm::perspective(glm::radians(player_camera.zoom), (float)screen_width / (float)screen_height, 0.1f, 100.0f);
    glm::mat4 view = player_camera.get_view_matrix();

    default_shader.set_mat4("projection", projection);
    default_shader.set_mat4("view", view);

    for (auto& tile : dungeon_tiles) {
        glm::mat4 model_matrix = glm::mat4(1.0f);
        model_matrix = glm::translate(model_matrix, tile.location);
        model_matrix = glm::scale(model_matrix, glm::vec3(0.1f, 0.1f, 0.1f));
        model_matrix = glm::rotate(model_matrix, tile.rotation * (float) (M_PI / 2), glm::vec3(0.0f, 1.0f, 0.0f));

        default_shader.set_mat4("model", model_matrix);
        tile.model.draw(default_shader);
    }

    SDL_GL_SwapWindow(window);

    return 1;
}

int Game::clean() {
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

    return 1;
}