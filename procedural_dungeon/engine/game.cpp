#include "game.h"

const GLchar* vertexShaderSource = "#version 410 core\n"
"layout (location = 0) in vec3 position;\n"
"layout (location = 1) in vec3 color;\n"
"out vec3 ourColor;\n"
"void main()\n"
"{\n"
"gl_Position = vec4(position, 1.0);\n"
"ourColor = color;\n"
"}\0";

const GLchar* fragmentShaderSource = "#version 410 core\n"
"in vec3 ourColor;\n"
"out vec4 color;\n"
"void main()\n"
"{\n"
"color = vec4(ourColor, 1.0f);\n"
"}\n\0";

GLfloat vertices[] = {
    // Positions            // Colors
    0.5f, -0.5f, 0.0f,      1.0f, 0.0f, 0.0f,  // Bottom Right
    -0.5f, -0.5f, 0.0f,     0.0f, 1.0f, 0.0f,  // Bottom Left
    0.0f, 0.5f, 0.0f,       0.0f, 0.0f, 1.0f   // Top
};

GLfloat colors[] = {
    1.0f, 0.0f, 0.0f, 1.0f,
    0.0f, 1.0f, 0.0f, 1.0f,
    0.0f, 0.0f, 1.0f, 1.0f
};

GLuint VBO, VAO;

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
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);
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
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    ///////////////////////////
    // REMOVE TO STANDARDIZE
    ///////////////////////////
    // Vertex Shaders Setup
    GLuint vertex_shader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex_shader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertex_shader);

    GLint success;
    GLchar info_log[512];
    glGetShaderiv(vertex_shader, GL_COMPILE_STATUS, &success);

    if (!success) {
        glGetShaderInfoLog(vertex_shader, 512, NULL, info_log);
        std::cout << "Vertex Shader Compilation Failed! Reason: " << std::endl
            << info_log << std::endl;
    }

    // Fragment Shaders Setup
    GLuint fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment_shader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragment_shader);

    glGetShaderiv(fragment_shader, GL_COMPILE_STATUS, &success);

    if (!success) {
        glGetShaderInfoLog(vertex_shader, 512, NULL, info_log);
        std::cout << "Fragment Shader Compilation Failed! Reason: " << std::endl
            << info_log << std::endl;
    }

    // Linking shaders
    shader_program = glCreateProgram();
    glAttachShader(shader_program, vertex_shader);
    glAttachShader(shader_program, fragment_shader);
    glLinkProgram(shader_program);

    glGetProgramiv(shader_program, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shader_program, 512, NULL, info_log);
        std::cout << "Shader Linking Failed! Reason: " << std::endl
            << info_log << std::endl;
    }

    glDeleteShader(vertex_shader);
    glDeleteShader(fragment_shader);

    // Vertex Data & Attribute Pointers
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    // Bind VAO, then bind & set vertex buffers and attribute pointers
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // Position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);
    // Color attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    ///////////////////////////
    // END REMOVE TO STANDARDIZE
    ///////////////////////////

    glViewport(0, 0, screen_width, screen_height);

    return 1;
}

void Game::quit() {
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteProgram(shader_program);

    SDL_GL_DeleteContext(context);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);

    SDL_Quit();
}

int Game::start() {
    // Useful code snippet for getting window size
    /*int width, height;

    SDL_GetWindowSize(window, &width, &height);*/
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
    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, 3);
    glBindVertexArray(0);

    SDL_GL_SwapWindow(window);

    return 1;
}

int Game::clean() {
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

    return 1;
}