#pragma once

#include <SFML/Graphics.hpp>
#include "tilemap.hpp"

struct Object {
public:
	Object(sf::Color color, float x, float y, float radius);
	void fall(Tilemap map, float gravity);
	void move(Tilemap map);
	bool supported(Tilemap map);
	bool collides(Object other);
	sf::Color color;
	float x, y, radius, xspeed, yspeed;
private:
	bool can_move(Tilemap map, float xoff, float yoff);
};
