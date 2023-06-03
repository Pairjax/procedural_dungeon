# CMakeList.txt : CMake project for procedural_dungeon, include source and define
# project specific logic here.
#

set(OpenGL_GL_PREFERENCE "GLVND")

find_package(SDL2 REQUIRED)
find_package(OpenGL REQUIRED COMPONENTS OpenGL)
find_package(GLEW REQUIRED)

# Add source to this project's executable.
add_executable (procedural_dungeon "procedural_dungeon.cpp"  "engine/game.h" "engine/game-object.h" "engine/game-object.cpp" "engine/camera.h" "engine/camera.cpp" "engine/lighting.h" "engine/lighting.cpp" "engine/material.h" "engine/material.cpp" "engine/model.h" "engine/model.cpp" "engine/shadow.h" "engine/shadow.cpp" "engine/game.cpp")

target_link_libraries(procedural_dungeon SDL2::SDL2 SDL2::SDL2main)
target_link_libraries(procedural_dungeon GLEW::GLEW)

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET procedural_dungeon PROPERTY CXX_STANDARD 20)
endif()

# TODO: Add tests and install targets if needed.