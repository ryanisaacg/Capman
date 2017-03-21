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

sf::Text scoreDisplay;	
int score = 0, health = 3, hurt_cooldown = 0, max_hurt_cooldown = 120;

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

void save_level(std::string filename, Object playerOut, std::vector<Object> enemies, Tilemap map) {
	std::ofstream output(filename);
	for(int y = 0; y < map.getHeight(); y += 32) {
		for(int x = 0; x < map.getWidth(); x += 32) {
			char out = ' ';
			if(!map.free(x, y)) {
				out = 'w';
			} else if(playerOut.contains(x, y)) {
				out = 'p';
			} else {
				for(auto enemy : enemies) {
					if(enemy.contains(x, y)) {
						out = 'e';
						break;
					}
				}
			}
			output << out;
		}
		output << std::endl;
	}
}

void spawn_pellets(Tilemap map, std::vector<Object> &pellets) {
	for(int i = 0; i < map.getWidth(); i += 32) 
		for(int j = 0; j < map.getHeight(); j += 32) 
			if(map.free(i, j))
				pellets.push_back(Object(sf::Color::White, i + 16, j + 16, 6, 0.5f));
}

static void edit(sf::Window &window, Tilemap map, Object &player, std::vector<Object> &enemies, bool justPressedE) {
	auto mouse_pos = sf::Mouse::getPosition(window);
	if(sf::Mouse::isButtonPressed(sf::Mouse::Button::Left)) {
		map.set(true, mouse_pos.x, mouse_pos.y);
	} else if(sf::Mouse::isButtonPressed(sf::Mouse::Button::Right)) {
		map.set(false, mouse_pos.x, mouse_pos.y);
		for(auto enemy = enemies.begin(); enemy < enemies.end(); enemy++)
			if(enemy->contains(mouse_pos.x, mouse_pos.y)) 
				enemy = enemies.erase(enemy);
	}
	if(sf::Keyboard::isKeyPressed(sf::Keyboard::P)) {
		player.x = mouse_pos.x;
		player.y = mouse_pos.y;
	}
	if(justPressedE)
		enemies.push_back(Object(sf::Color::Red, mouse_pos.x, mouse_pos.y, 10));
}

static void update(Tilemap map, Object &player, std::vector<Object> &pellets, std::vector<Object> &ghostPellets, std::vector<Object> &enemies) {
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
}

int main() {
	Tilemap map(game_width, game_height, 32, 32);
	Object player(sf::Color::Cyan, 0, 0, 10);
	std::vector<Object> pellets, ghostPellets, enemies;
    sf::RenderWindow window(sf::VideoMode(window_width, window_height), "SFML works!");
	window.setKeyRepeatEnabled(false);
	sf::Clock clock;
	sf::Font font;
	font.loadFromFile("arial.ttf");
	scoreDisplay = sf::Text("", font);
	scoreDisplay.setOutlineColor(sf::Color::Yellow);
	scoreDisplay.setFillColor(sf::Color::White);
	scoreDisplay.setOutlineThickness(0.5);
	bool editing = false;
	bool previous_switch = false;
	bool saving = false;
	std::string save_name = "";

	load_level("stest", player, enemies, map);
	spawn_pellets(map, pellets);

    while (window.isOpen()) {
        sf::Event event;
		bool justPressedE = false;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
			if(saving) {
				if(event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Key::Return) {
					save_level(save_name, player, enemies, map); 
					save_name = "";
				} else if(event.type == sf::Event::TextEntered) {
					save_name += event.text.unicode;
				}
			} else {
				if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Key::E)
					justPressedE = true;
				if(event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Key::S) 
					saving = true;
			}
        }
		if(sf::Keyboard::isKeyPressed(sf::Keyboard::PageUp)) {
			if(!previous_switch) { 
				editing = !editing;
				previous_switch = true;
			}
		} else {
			previous_switch = false;
		}
		
		if(editing) {
			edit(window, map, player, enemies, justPressedE); 
		} else {
			//Update the game
			update(map, player, pellets, ghostPellets, enemies) ;
		}
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
