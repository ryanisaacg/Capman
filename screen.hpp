#pragma once

#include <SFML/Graphics.hpp>

class Screen {
public:
	virtual bool tick(sf::RenderWindow &window) = 0;
};
