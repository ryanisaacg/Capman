#pragma once

class Tilemap {
public:
	Tilemap(int width, int height, int tile_width, int tile_height);
	void set(int data, float x, float y);
	int get(float x, float y);
	bool valid(float x, float y);
	bool free(float x, float y);
	bool free(float x, float y, float width, float height);
	int getWidth();
	int getHeight();
private:
	int get_index(float x, float y);
	int width, height, tile_width, tile_height;
	int *buffer;
};
