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
#include "music.hpp"
#include "level.hpp"
#include "render.hpp"
#include "tilemap.hpp"

sf::Text scoreDisplay;	
int score = 0, health = 3, hurt_cooldown = 0, max_hurt_cooldown = 120;
sf::Sound pickup, jump, hurt;

void spawn_pellets(Tilemap map, std::vector<Object> &pellets) {
	for(int i = 0; i < map.getWidth(); i += 32) 
		for(int j = 0; j < map.getHeight(); j += 32) 
			if(map.free(i, j) && rand() < RAND_MAX / 7)
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
		enemies.push_back(Object(sf::Color::Red, mouse_pos.x, mouse_pos.y, 16));
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
	if(sf::Keyboard::isKeyPressed(sf::Keyboard::W) && player.supported(map)) {
		player.yspeed = -12;
		jump.play();
	}
	//Update all of the pellets
	for(auto pellet = pellets.begin(); pellet < pellets.end(); pellet++) {
		if(pellet->collides(player)) {
			pickup.play();
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
			enemy.xspeed = 0.75;
		} else if(enemy.x > player.x) {
			enemy.xspeed = -0.75;
		}
		if(enemy.yspeed < 8 && enemy.y > player.y) {
			enemy.yspeed = -2;
		}
		enemy.fall(map, 0.25);
		if(hurt_cooldown == 0 && enemy.collides(player)) {
			hurt_cooldown = max_hurt_cooldown;
			health -= 1;
			hurt.play();
		}
	}
	if(hurt_cooldown > 0) hurt_cooldown--;
}

int main() {
	Tilemap map(game_width, game_height, 32, 32);
	Object player(sf::Color::Cyan, 0, 0, 16);
	std::vector<Object> pellets, ghostPellets, enemies;
    sf::RenderWindow window(sf::VideoMode(window_width, window_height), "CapMan");
	window.setKeyRepeatEnabled(false);
	window.setFramerateLimit(60);
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
	std::vector<std::string> level_names = load_level_list();
	MusicPlayer music;
	unsigned int level_index = 0;

	hurt.loadFromFile("hurt.wav");
	jump.loadFromFile("jump.wav");
	pickup.loadFromFile("pickup.wav");

	srand(time(nullptr));

	load_level(level_names[0], player, enemies, map, health);
	spawn_pellets(map, pellets);

    while (window.isOpen()) {
        sf::Event event;
		bool justPressedE = false;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
			if (!window.hasFocus()) continue;
			if(saving) {
				if(event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Key::Return) {
					save_level(save_name, player, enemies, map); 
					save_name = "";
					saving = false;
				} else if(event.type == sf::Event::TextEntered) {
					if(event.text.unicode != 8) {
						save_name += event.text.unicode;
					} else {
						save_name = save_name.substr(0, save_name.size() - 1);
					}
					scoreDisplay.setString(save_name);
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

		if(pellets.empty()) {
			level_index++;
			if(level_index >= level_names.size()) {
				load_level(level_names[level_index], player, enemies, map, health);
				spawn_pellets(map, pellets);
			} else {
				scoreDisplay.setString("You Win!");
			}
		}

		if(health == 0) {
			load_level(level_names[level_index], player, enemies, map, health);
		}
		music.update();
    }
    return 0;
}
