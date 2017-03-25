#pragma once

#include <SFML/Graphics.hpp>
#include "tilemap.hpp"

struct Object {
public:
	Object(const Object &obj);
	Object(sf::Color color, float x, float y, float radius, float scale = 1);
	void fall(Tilemap map, float gravity);
	void move(Tilemap map);
	bool supported(Tilemap map);
	bool contains(int x, int y);
	bool collides(Object other);
	void draw(sf::RenderWindow &window);
	sf::Color color;
	float x, y, radius, xspeed, yspeed, draw_scale;
	bool flip_x;
private:
	bool can_move(Tilemap map, float xoff, float yoff);
};
