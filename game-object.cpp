#include "game-object.hpp"

#include <cmath>
#include <iostream>

Object::Object(const Object &obj) : 
	color(obj.color), x(obj.x), y(obj.y), radius(obj.radius), xspeed(obj.xspeed), yspeed(obj.yspeed), draw_scale(obj.draw_scale) {}

Object::Object(sf::Color color, float x, float y, float radius, float scale ) :
	color(color), x(x), y(y), radius(radius), xspeed(0), yspeed(0), draw_scale(scale) {}

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
	} else {
		xspeed = 0;
		yspeed = 0;
	}
}

bool Object::supported(Tilemap map) {
	return !can_move(map, 0, 1);
}
	
bool Object::contains(int px, int py) {
	float dx = x - px;
	float dy = y - py;
	return (dx * dx + dy * dy) <= radius * radius;
}
	
bool Object::collides(Object other) {
	float radiusSum = radius + other.radius;
	float dx = x - other.x;
	float dy = y - other.y;
	return (dx * dx + dy * dy) <= radiusSum * radiusSum;
}

bool Object::can_move(Tilemap map, float xoff, float yoff) {
	return map.free(x - radius + xoff, y - radius + yoff, radius * 2, radius * 2);
}

void Object::draw(sf::RenderWindow &window) {
	sf::CircleShape shape(radius * draw_scale);
	shape.setPosition(x - radius, y - radius);
	shape.setFillColor(color);
	window.draw(shape);
}
