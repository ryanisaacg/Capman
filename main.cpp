#include <cmath>
#include <fstream>
#include <iostream>
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <string>
#include <vector>

#include "game-object.hpp"
#include "main.hpp"
#include "render.hpp"
#include "tilemap.hpp"


void load_level(std::string filename, Object &playerOut, std::vector<Object> &enemiesOut, Tilemap &mapOut) {
	std::ifstream input(filename);
	int y = 0;
	while(!input.eof()) {
		std::string line;
		getline(input, line);
		for(unsigned int i = 0; i < line.size(); i++) {
			int x = i * 32;
			switch(line[i]) {
				case 'P':
				case 'p':
					playerOut = Object(sf::Color::Cyan, x, y, 10);
					break;
				case 'W':
				case 'w':
					mapOut.set(1, x, y);
					break;
				case 'E':
				case 'e':
					enemiesOut.push_back(Object(sf::Color::Red, x, y, 10));
					break;
			}
		}
		y += 32;
	}
	input.close();
}

void spawn_pellets(Tilemap map, std::vector<Object> &pellets) {
	for(int i = 0; i < map.getWidth(); i += 32) 
		for(int j = 0; j < map.getHeight(); j += 32) 
			if(map.free(i, j))
				pellets.push_back(Object(sf::Color::White, i + 16, j + 16, 6, 0.5f));
}

int main() {
	Tilemap map(game_width, game_height, 32, 32);
    sf::RenderWindow window(sf::VideoMode(window_width, window_height), "SFML works!");
	Object player(sf::Color::Cyan, 0, 0, 10);
	sf::Clock clock;
	std::vector<Object> pellets, ghostPellets, enemies;
	sf::Font font;
	font.loadFromFile("arial.ttf");
	sf::Text scoreDisplay("", font);
	scoreDisplay.setOutlineColor(sf::Color::Yellow);
	scoreDisplay.setFillColor(sf::Color::White);
	scoreDisplay.setOutlineThickness(0.5);
	int score = 0, health = 3, hurt_cooldown = 0, max_hurt_cooldown = 120;

	load_level("level1", player, enemies, map);
	spawn_pellets(map, pellets);

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
        }
		//Walk
		if(sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
			player.xspeed = -4;
		} else if(sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
			player.xspeed = 4;
		} else {
			player.xspeed = 0;
		}
		player.fall(map, 0.5);
		//Jump
		if(sf::Keyboard::isKeyPressed(sf::Keyboard::W) && player.supported(map))
			player.yspeed = -12;
		//Update all of the pellets
		for(auto pellet = pellets.begin(); pellet < pellets.end(); pellet++) {
			if(pellet->collides(player)) {
				ghostPellets.push_back(Object(sf::Color::Yellow, player.x, player.y, pellet->radius * 2));
				pellet = pellets.erase(pellet);
				score++;
				scoreDisplay.setString(std::to_string(score));
				scoreDisplay.setPosition(window_width / 2 - scoreDisplay.getLocalBounds().width / 2, 0);
			}
		}
		//Update the ghost pellet effect
		for(auto ghost = ghostPellets.begin(); ghost < ghostPellets.end(); ghost++) {
			if(ghost->radius <= 0) {
				ghost = ghostPellets.erase(ghost);
			} else {
				ghost->radius -= 0.5;
				ghost->color.a -= 10;
			}
		}
		//Move the enemies towards the player
		for(auto &enemy : enemies) {
			if(abs(enemy.x - player.x) < 2) {
				enemy.x = player.x;
				enemy.xspeed = 0;
			} else if(enemy.x < player.x) {
				enemy.xspeed = 2;
			} else if(enemy.x > player.x) {
				enemy.xspeed = -2;
			}
			if(enemy.yspeed < 8 && enemy.y > player.y) {
				enemy.yspeed = -4;
			}
			enemy.fall(map, 0.25);
			if(hurt_cooldown == 0 && enemy.collides(player)) {
				hurt_cooldown = max_hurt_cooldown;
				health -= 1;
			}
		}
		if(hurt_cooldown > 0) hurt_cooldown--;
		//Render the game
		render_state(window, map, player, enemies, pellets, ghostPellets, scoreDisplay, health);
		//Handle frame timing
		sf::Time sleepTime = sf::milliseconds(16) - clock.getElapsedTime();
		if(sleepTime > sf::milliseconds(1))
			sf::sleep(sleepTime);
		clock.restart();
    }
    return 0;
}
