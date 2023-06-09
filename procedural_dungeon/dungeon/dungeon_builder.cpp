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

	// Rotate 90 degrees for each turn
	for (int i = 0; i < turns; i++) {
		for (glm::vec2 square : last_tile.filled_squares) {
			// 2D vector rotated 90 degrees is (-y, x)
			glm::vec2 new_square = glm::vec2(-square.y, square.x);

			// Cardinal directions of doors in this square are rotated over as well
			std::vector<Door_Direction> new_square_doors;
			for (auto& door : tile.square_doors[std::pair(square.x, square.y)]) {
				new_square_doors.push_back((Door_Direction) (door + 1));
			}

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
			Door_Direction direction = close_adjacent_door(door_direction, global_coords);

			if (direction != EMPTY) {
				open_directions.push_back(direction);
			}
		}

		free_doors.push_back(std::pair(global_coords, open_directions));
	}
}

Door_Direction DungeonBuilder::close_adjacent_door(Door_Direction door_direction, glm::vec2 center) {
	glm::vec2 adjacent_square = center + direction[door_direction];

	bool is_adjacent_empty = dungeon_map[(int)adjacent_square.x]
		[(int)adjacent_square.y][0] == EMPTY;

	if (is_adjacent_empty) {
		// If no adjacents, mark a new open door.
		return door_direction;
	} else {
		// Otherwise, ensure doors that are adjacent are properly closed.
		Door_Direction opposite = (Door_Direction)((door_direction + 2) % 4);
		std::pair closed_door = std::pair(adjacent_square, opposite);

		for (int i = 0; i < free_doors.size(); i++) {
			std::pair square = free_doors[i];

			if (square.first != adjacent_square) { continue; }

			std::vector<Door_Direction> open_doors = square.second;
			free_doors[i].second.erase(std::remove(open_doors.begin(), open_doors.end(), opposite));
		}
	}

	return EMPTY;
}

DungeonTile DungeonBuilder::get_random_tile() {
	int index = std::rand() % tile_pool.size();
	return tile_pool[index];
}
