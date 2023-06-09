# CMakeList.txt : CMake project for procedural_dungeon, include source and define
# project specific logic here.
#

set(OpenGL_GL_PREFERENCE "GLVND")

# Add source to this project's executable.
add_executable (procedural_dungeon "procedural_dungeon.cpp"  "engine/game.h"   "engine/camera.h" "engine/camera.cpp"   "engine/material.h" "engine/material.cpp" "engine/model.h" "engine/model.cpp"   "engine/game.cpp" "engine/shader.h" "engine/shader.cpp" "engine/mesh.h" "engine/mesh.cpp" "externals/stb_image.cpp" "dungeon/dungeon_builder.h" "dungeon/dungeon_builder.cpp")

find_package(SDL2 REQUIRED)
target_link_libraries(procedural_dungeon SDL2::SDL2 SDL2::SDL2main)

find_package(OpenGL REQUIRED COMPONENTS OpenGL)
target_link_libraries(procedural_dungeon ${OPENGL_LIBRARY})

find_package(GLEW REQUIRED)
target_link_libraries(procedural_dungeon GLEW::GLEW)

find_package(glm REQUIRED)
target_link_libraries(procedural_dungeon glm::glm)

include_directories(S:/sdks/assimp/include)
add_subdirectory(S:/sdks/assimp assimp/)
target_link_libraries(procedural_dungeon assimp)

set(ZLIB_LIBRARIES zlibstatic)
set(ENABLE_BOOST_WORKAROUND ON)
set(BUILD_STATIC_LIB ON)
set(BUILD_ASSIMP_TOOLS  ON)
set(ASSIMP_BUILD_STATIC_LIB ON)

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET procedural_dungeon PROPERTY CXX_STANDARD 20)
endif()