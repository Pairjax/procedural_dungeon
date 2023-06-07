# CMakeList.txt : CMake project for procedural_dungeon, include source and define
# project specific logic here.
#

set(OpenGL_GL_PREFERENCE "GLVND")

# Add source to this project's executable.
add_executable (procedural_dungeon "procedural_dungeon.cpp"  "engine/game.h" "engine/game-object.h" "engine/game-object.cpp" "engine/camera.h" "engine/camera.cpp" "engine/lighting.h" "engine/lighting.cpp" "engine/material.h" "engine/material.cpp" "engine/model.h" "engine/model.cpp" "engine/shadow.h" "engine/shadow.cpp" "engine/game.cpp" "engine/shader.h" "engine/shader.cpp")

find_package(SDL2 REQUIRED)
target_link_libraries(procedural_dungeon SDL2::SDL2 SDL2::SDL2main)

find_package(OpenGL REQUIRED COMPONENTS OpenGL)
target_link_libraries(procedural_dungeon ${OPENGL_LIBRARY})

find_package(GLEW REQUIRED)
target_link_libraries(procedural_dungeon GLEW::GLEW)

find_package(glm REQUIRED)
target_link_libraries(procedural_dungeon glm::glm)

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET procedural_dungeon PROPERTY CXX_STANDARD 20)
endif()