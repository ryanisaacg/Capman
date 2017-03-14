#include "game-object.hpp"

#include <cmath>
#include <iostream>

Object::Object(sf::Color color, float x, float y, float radius) :
	color(color), x(x), y(y), radius(radius), xspeed(0), yspeed(0) {}

void Object::fall(Tilemap map, float gravity) {
	yspeed += gravity;
	move(map);
}

void Object::move(Tilemap map) {
	if(can_move(map, xspeed, yspeed)) {
		x += xspeed;
		y += yspeed;
	} else if(can_move(map, xspeed, 0)) {
		if(fabs(yspeed) >= 1) {
			yspeed /= 2;
			move(map);
		} else {
			x += xspeed;
			yspeed = 0;
		}
	} else if(can_move(map, 0, yspeed)) {
		if(fabs(xspeed) >= 1) {
			xspeed /= 2;
			move(map);
		} else {
			y += yspeed;
			xspeed = 0;
		}
	}
}

bool Object::can_move(Tilemap map, float xoff, float yoff) {
	return map.free(x - radius + xoff, y - radius + yoff, radius * 2, radius * 2);
}