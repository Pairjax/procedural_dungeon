#include "dungeon_builder.h"

DungeonBuilder::DungeonBuilder() {
	for (int i = 0; i < DUNGEON_MAP_WIDTH; i++) {
		for (int j = 0; j < DUNGEON_MAP_HEIGHT; j++) {
			dungeon_map[i][j].push_back(NO_SQUARE);
		}
	}
}

std::vector<DungeonTile> DungeonBuilder::generate_dungeon() {
	glm::vec2 start_indexes = glm::vec2(DUNGEON_MAP_WIDTH / 2, DUNGEON_MAP_HEIGHT / 2);

	DungeonTile tile = get_random_tile();
	imprint_tile(tile, start_indexes);
	tiles.push_back(std::pair(start_indexes, tile));


	for (int i = 0; i < TILE_MAX - 1; i++) {
		tile = get_random_tile();

		bool tile_found = false;
		for (int j = 0; j < free_doors.size(); j++) {
			glm::vec2 square_position = free_doors[j].first;

			for (Door_Direction square_door : free_doors[j].second) {
				// Check for valid position
				std::pair<glm::vec2, DungeonTile> final_tile = get_valid_tile_position(square_door, square_position, tile);

				// Check if no valid spot was found
				if (final_tile.first.x == -1) { 
					continue; 
				}

				tile_found = true;
				imprint_tile(final_tile.second, final_tile.first);
				tiles.push_back(std::pair(final_tile.first, final_tile.second));
				break;
			}

			if (tile_found) { break; }
		}
	}

	std::vector<DungeonTile> dungeon;
	for (int i = 0; i < tiles.size(); i++) {
		DungeonTile tile = tiles[i].second;

		// translates the 2d vector to 3d in front of camera
		glm::vec3 tile_location = glm::vec3((tiles[i].first.x-25.0f)*0.2f, -1.0f, (tiles[i].first.y - 38.0f) * 0.2f);

		tile.location = tile_location;

		dungeon.push_back(tile);
	}

	return dungeon;
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
				new_square_doors.push_back((Door_Direction) ((door + 1) % DOOR_SIDES));
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

void DungeonBuilder::imprint_tile(DungeonTile tile, glm::vec2 location) {
	for (glm::vec2 square : tile.filled_squares) {
		std::vector<Door_Direction> doors = tile.square_doors[std::pair((int)square.x, (int)square.y)];
		glm::vec2 global_coords = location + square;
		dungeon_map[(int) global_coords.x][(int) global_coords.y] = doors;

		std::vector<Door_Direction> open_directions;
		for (auto& door_direction : doors) {
			Door_Direction final_door_direction = close_adjacent_door(door_direction, global_coords);

			// If no adjacent door was found, instead mark a new open door.
			if (final_door_direction != NO_SQUARE) {
				open_directions.push_back(final_door_direction);
			}
		}

		free_doors.push_back(std::pair(global_coords, open_directions));
	}
}

Door_Direction DungeonBuilder::close_adjacent_door(Door_Direction door_direction, glm::vec2 center) {
	glm::vec2 adjacent_square = center + direction[door_direction];

	std::vector<Door_Direction> adjacent_doors = dungeon_map[(int)adjacent_square.x]
		[(int)adjacent_square.y];

	if (adjacent_doors.empty()) { 
		std::cout << "Error! Invalid state reached. Cannot have a square with no doors." << std::endl;
	}

	bool is_adjacent_empty = dungeon_map[(int)adjacent_square.x]
		[(int)adjacent_square.y][0] == NO_SQUARE;

	if (is_adjacent_empty) {
		// If no adjacents, return a new open door.
		return door_direction;
	} else {
		// Otherwise, ensure doors that are adjacent are properly closed.
		Door_Direction opposite = (Door_Direction)((door_direction + 2) % DOOR_SIDES);
		std::pair closed_door = std::pair(adjacent_square, opposite);

		for (int i = 0; i < free_doors.size(); i++) {
			std::pair square = free_doors[i];

			if (square.first != adjacent_square) { continue; }

			std::vector<Door_Direction> open_doors = square.second;
			for (int j = 0; j < open_doors.size(); j++) {
				if (open_doors[j] == opposite) {
					free_doors[i].second.erase(free_doors[i].second.begin() + j);
				}
			}
		}
	}

	return NO_SQUARE;
}

std::pair<glm::vec2, DungeonTile> DungeonBuilder::get_valid_tile_position(
													Door_Direction target_door_direction, 
													glm::vec2 target_door_location,
													DungeonTile tile) {
	glm::vec2 free_square = target_door_location + direction[target_door_direction];
	Door_Direction opposite = (Door_Direction)((target_door_direction + 2) % DOOR_SIDES);

	for (int i = 0; i < DOOR_SIDES; i++) {
		bool is_connected_square_valid = true;
		
		for (auto& connected_square : tile.filled_squares) {
			std::vector<Door_Direction> doors = tile.square_doors[
				std::pair((int)connected_square.x, (int)connected_square.y)
			];

			bool opposite_door_found = false;
			for (Door_Direction door : doors) {
				if (door == opposite) {
					opposite_door_found = true;

					// Check if all other squares are valid
					for (auto& tile_square : tile.filled_squares) {
						glm::vec2 tile_square_location = tile_square + free_square;

						bool square_valid = is_square_valid(tile_square_location,
								tile.square_doors[std::pair(tile_square.x, tile_square.y)]);
						if (!square_valid) {
							is_connected_square_valid = false;
							break;
						}
					}
				}
			}

			if (is_connected_square_valid && opposite_door_found) {
				tile.rotation = i;
				glm::vec2 coord = free_square - connected_square;
				return std::pair(coord, tile);
			}
		}
		tile = rotate_tile(tile, 1);
	}

	// Fail; no valid position found
	return std::pair(glm::vec2(-1,-1), tile);
}

bool DungeonBuilder::is_square_valid(glm::vec2 square_location, std::vector<Door_Direction> doors)
{
	std::vector<Door_Direction> occupied_square = dungeon_map[(int)square_location.x][(int)square_location.y];
	if (occupied_square.empty()) { return false; } // Checks if the square is empty of doors, which means it exists
	if (occupied_square[0] != NO_SQUARE) { return false; } // A square exists here

	for (int i = 0; i < DOOR_SIDES; i++) {
		Door_Direction facing_direction = (Door_Direction)i;
		Door_Direction opposite = (Door_Direction)((facing_direction + 2) % DOOR_SIDES);

		glm::vec2 adjacent_square = square_location + direction[i];
		std::vector<Door_Direction> adjacent_square_doors = dungeon_map[(int)adjacent_square.x][(int)adjacent_square.y];

		bool adjacent_door_exists = false;
		bool adjacent_square_exists = false;

		// Checking if square has no doors.
		if (adjacent_square_doors.empty()) { 
			adjacent_square_exists = true;
		}

		for (Door_Direction adjacent_square_door : adjacent_square_doors) {
			if (adjacent_square_door == NO_SQUARE) {
				break;
			}

			adjacent_square_exists = true;

			if (adjacent_square_door == opposite) {
				adjacent_door_exists = true;
				break;
			}
		}

		if (!adjacent_square_exists) { continue; }

		bool door_exists = false;
		for (Door_Direction door : doors) {
			if (door == facing_direction) {
				door_exists = true;
			}
		}

		if (!door_exists && adjacent_door_exists) { return false; }
		if (door_exists && !adjacent_door_exists) { return false; }
	}
	return true;
}

DungeonTile DungeonBuilder::get_random_tile() {
	int index = std::rand() % tile_pool.size();
	return tile_pool[index];
}
