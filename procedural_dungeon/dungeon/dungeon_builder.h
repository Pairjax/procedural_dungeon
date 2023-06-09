#include "../engine/model.h"

#include <glm/glm.hpp>

#include <string>
#include <vector>
#include <utility>
#include <map>
#include <algorithm>

#define DUNGEON_MAP_WIDTH 50
#define DUNGEON_MAP_HEIGHT 50

#define TILE_MAX 15

// Access using Dungeon_Door_Direction
const glm::vec2 direction[4] = {
	glm::vec2(0, 1),
	glm::vec2(0, -1),
	glm::vec2(1, 0),
	glm::vec2(-1, 0),
};

enum Door_Direction {
	NORTH = 0,
	SOUTH = 1,
	EAST = 2,
	WEST = 3,
	EMPTY = 4, // Reserved for empty squares
};

struct DungeonTile {
	Model model;
	// Squares are single 1x1 spaces in the dungeon
	std::vector<glm::vec2> filled_squares;
	// Each square in the dungeon may have 0-4 doors. Connected squares do not count.
	std::map<std::pair<int, int>, std::vector<Door_Direction>> square_doors;
};

class DungeonBuilder {
public:
	DungeonBuilder();

	std::vector<std::pair<Model, glm::vec3>> generate_dungeon();

	void add_tile(DungeonTile tile);

private:
	// Pool of all possible tiles
	std::vector<DungeonTile> tile_pool;

	// We only need to know what doors are in a given squrae. Null = No tile there.
	std::vector<Door_Direction> dungeon_map[DUNGEON_MAP_HEIGHT][DUNGEON_MAP_WIDTH];

	// A list of every coordinate with open doors.
	std::vector<std::pair<glm::vec2, std::vector<Door_Direction>>> free_doors;

	// Turns are in 90 degrees clockwise
	DungeonTile rotate_tile(DungeonTile tile, int turns);

	void imprint_tile(DungeonTile tile, glm::vec2 location);

	DungeonTile get_random_tile();
};