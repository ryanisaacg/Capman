#pragma once

#include <SFML/Graphics.hpp>
#include <vector>

#include "game-object.hpp"
#include "tilemap.hpp"

void render_state(sf::RenderWindow &window, Tilemap map, Object player, std::vector<Object> enemies, 
		std::vector<Object> pellets, std::vector<Object> ghostPellets, sf::Text &score);
