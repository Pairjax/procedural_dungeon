# CMakeList.txt : CMake project for procedural_dungeon, include source and define
# project specific logic here.
#

find_package(SDL2 REQUIRED)

# Add source to this project's executable.
add_executable (procedural_dungeon "procedural_dungeon.cpp" "procedural_dungeon.h")

target_link_libraries(procedural_dungeon SDL2::SDL2 SDL2::SDL2main)

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET procedural_dungeon PROPERTY CXX_STANDARD 20)
endif()

# TODO: Add tests and install targets if needed.