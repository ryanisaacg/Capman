#include <algorithm>

#include "tilemap.hpp"

Tilemap::Tilemap(int width, int height, int tile_width, int tile_height) 
		: width(width), height(height), tile_width(tile_width), tile_height(tile_height),buffer(new int[width * height]) {
	std::fill(buffer, buffer + width * height, 0);
}
int Tilemap::get_index(float x, float y) {
	int xSpot = (int)(x * tile_width / width);
	int ySpot = (int)(y * tile_height / height);
	return buffer[xSpot * height + ySpot];
}

void Tilemap::set(int data, float x, float y) {
	buffer[get_index(x, y)] = data;
}

int Tilemap::get(float x, float y) {
	return buffer[get_index(x, y)];
}


bool Tilemap::valid(float x, float y) {
	return x >= 0 && y >= 0 && x < width && y < height;
}

bool Tilemap::free(float x, float y) {
	return valid(x, y) && get(x, y) != 0;
}

bool Tilemap::free(float x, float y, float width, float height) {
	int lowestX = (int)(x * tile_width / this->width);
	int lowestY = (int)(y * tile_height / this->height);
	int highestX = (int)((x + width) * tile_height / this->width + 0.5f);
	int highestY = (int)((y + height) * tile_height / this->height + 0.5f);
	if(!valid(x, y) || !valid(x + width, y + height)) {
		return false;
	}
	for(int i = lowestX; i < highestX; i++) 
		for(int j = lowestY; j < highestY; j++) 
			if(buffer[i * this->height + j])
				return false;
	return true;
}
