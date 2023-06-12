#include "../engine/model.h"

#include <glm/glm.hpp>

#include <string>
#include <vector>
#include <utility>
#include <map>
#include <algorithm>

#define DUNGEON_MAP_WIDTH 50
#define DUNGEON_MAP_HEIGHT 50

#define DOOR_SIDES 4

#define TILE_MAX 15

// Access using Dungeon_Door_Direction
const glm::vec2 direction[4] = {
	glm::vec2(0, 1),
	glm::vec2(1, 0),
	glm::vec2(0, -1),
	glm::vec2(-1, 0),
};

enum Door_Direction {
	NORTH = 0,
	EAST = 1,
	SOUTH = 2,
	WEST = 3,
	NO_SQUARE = 4, // Reserved for empty squares
};

struct DungeonTile {
	Model model;
	// Squares are single 1x1 spaces in the dungeon
	std::vector<glm::vec2> filled_squares;
	// Each square in the dungeon may have 0-4 doors. Connected squares do not count.
	std::map<std::pair<int, int>, std::vector<Door_Direction>> square_doors;
	// Ending location
	glm::vec3 location;
	// Ending rotation in 90 degree increments;
	int rotation;
};

class DungeonBuilder {
public:
	DungeonBuilder();

	std::vector<DungeonTile> generate_dungeon();

	void add_tile(DungeonTile tile);

private:
	// Pool of all possible tiles
	std::vector<DungeonTile> tile_pool;

	// We only need to know what doors are in a given squrae.
	std::vector<Door_Direction> dungeon_map[DUNGEON_MAP_WIDTH][DUNGEON_MAP_HEIGHT];

	// A list of every coordinate with open doors.
	std::vector<std::pair<glm::vec2, std::vector<Door_Direction>>> free_doors;

	std::vector<std::pair<glm::vec2, DungeonTile>> tiles;

	// Turns are in 90 degrees clockwise
	DungeonTile rotate_tile(DungeonTile tile, int turns);

	void imprint_tile(DungeonTile tile, glm::vec2 location);

	Door_Direction close_adjacent_door(Door_Direction door_direction, glm::vec2 center);

	std::pair<glm::vec2, DungeonTile> get_valid_tile_position(Door_Direction door_direction, glm::vec2 center,
										DungeonTile tile);

	bool is_square_valid(glm::vec2 room_location, std::vector<Door_Direction> doors);

	DungeonTile get_random_tile();
};