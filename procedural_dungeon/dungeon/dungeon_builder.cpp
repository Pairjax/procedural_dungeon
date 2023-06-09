#include "dungeon_builder.h"

DungeonBuilder::DungeonBuilder() {

}

std::vector<std::pair<Model, glm::vec3>> DungeonBuilder::generate_dungeon() {
	glm::vec2 start_indexes = glm::vec2(DUNGEON_MAP_HEIGHT / 2, DUNGEON_MAP_WIDTH / 2);

	imprint_tile(get_random_tile(), start_indexes);

	for (int i = 0; i < TILE_MAX - 1; i++) {

	}

	return std::vector<std::pair<Model, glm::vec3>>();
}

void DungeonBuilder::add_tile(DungeonTile tile) {
	tile_pool.push_back(tile);
}

DungeonTile DungeonBuilder::rotate_tile(DungeonTile tile, int turns) {
	DungeonTile last_tile = tile;
	DungeonTile current_tile = tile;
	current_tile.filled_squares.clear();
	current_tile.square_doors.clear();

	for (int i = 0; i < turns; i++) {
		for (glm::vec2 square : last_tile.filled_squares) {
			glm::vec2 new_square = glm::vec2(-square.y, square.x);
			std::vector<Door_Direction> new_square_doors = tile.square_doors[std::pair(square.x, square.y)];

			current_tile.filled_squares.push_back(new_square);
			current_tile.square_doors[std::pair(new_square.x, new_square.y)] = new_square_doors;
		}

		last_tile = current_tile;
		current_tile.filled_squares.clear();
		current_tile.square_doors.clear();
	}

	return last_tile;
}

void DungeonBuilder::imprint_tile(DungeonTile tile, glm::vec2 location)
{
	for (glm::vec2 square : tile.filled_squares) {
		std::vector<Door_Direction> doors = tile.square_doors[std::pair(square.x, square.y)];
		glm::vec2 global_coords = location + square;
		dungeon_map[(int) global_coords.x][(int) global_coords.y] = doors;

		std::vector<Door_Direction> open_directions;
		for (auto& door_direction : doors) {
			glm::vec2 adjacent_square = global_coords + direction[door_direction];

			bool is_adjacent_empty = dungeon_map[(int)adjacent_square.x]
												[(int)adjacent_square.y][0] == EMPTY;
			
			if (is_adjacent_empty) {
				// If no adjacents, mark a new open door.
				open_directions.push_back(door_direction);
			} else {
				// Otherwise, ensure doors that are adjacent are properly closed.
				Door_Direction opposite = (Door_Direction) ((door_direction + 2) % 4);
				std::pair closed_door = std::pair(adjacent_square, opposite);

				free_doors.erase(std::remove(free_doors.begin(), free_doors.end(), closed_door), free_doors.end());
			}
		}

		free_doors.push_back(std::pair(global_coords, open_directions));
	}
}

DungeonTile DungeonBuilder::get_random_tile()
{
	int index = std::rand() % tile_pool.size();
	return tile_pool[index];
}
