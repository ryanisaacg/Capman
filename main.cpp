#include "main.hpp"

#include <cmath>
#include <fstream>
#include <iostream>
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <string>
#include <vector>

#include "game.hpp"
#include "object.hpp"
#include "music.hpp"
#include "level.hpp"
#include "render.hpp"
#include "screen.hpp"
#include "start.hpp"
#include "tilemap.hpp"


int main() {
	srand(time(nullptr));
    
	sf::RenderWindow window(sf::VideoMode(window_width, window_height), "CapMan");
	window.setKeyRepeatEnabled(false);
	window.setFramerateLimit(60);

	std::vector<Screen*> screens;
	StartScreen start;
	screens.push_back(&start);
	Game game;	
	screens.push_back(&game);
	auto screen = screens.begin();
    
	while (window.isOpen()) {
		if((**screen).tick(window)) {
			screen++;
		}
	}
    return 0;
}
