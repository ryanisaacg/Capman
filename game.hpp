#pragma once

#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
#include <string>
#include <vector>

#include "object.hpp"
#include "music.hpp"
#include "screen.hpp"
#include "tilemap.hpp"

class Game : public Screen {
public:
	Game();
	bool tick(sf::RenderWindow &window);
private:
	void spawn_pellets();
	void edit(sf::Window &window, bool justPressedE);
	void update();
	void render_state(sf::RenderWindow &window);

	//Game simulation
	Tilemap map;
	Object player;
	std::vector<Object> pellets, enemies, ghostPellets;
	int score = 0, health = 3, hurt_cooldown = 0, max_hurt_cooldown = 120;
	std::vector<std::string> level_names;
	unsigned int level_index = 0;
	//Score Displaying
	sf::Font font;
	sf::Text scoreDisplay;	
	//Level editing
	bool editing = false, previous_switch = false, saving = false;
	std::string save_name;
	// Game sounds
	sf::Sound pickupSound, jumpSound, hurtSound, killSound;
	sf::SoundBuffer pickup, jump, hurt, kill;
	MusicPlayer music;
};
