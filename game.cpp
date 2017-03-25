#include "game.hpp"

#include "level.hpp"
#include "main.hpp"

sf::Texture player_walk_tex, player_fall_tex, player_jump_tex;

sf::Texture Game::load_texture(std::string filename) {
	sf::Texture tex;
	tex.loadFromFile(filename);
	return tex;
}

Game::Game() : map(game_width, game_height, 32, 32), player(sf::Color::Cyan, 0, 0, 13) {
	font.loadFromFile("arial.ttf");
	scoreDisplay = sf::Text("", font);
	scoreDisplay.setOutlineColor(sf::Color::Yellow);
	scoreDisplay.setFillColor(sf::Color::White);
	scoreDisplay.setOutlineThickness(0.5);
	
	std::string save_name = "";
	std::vector<std::string> level_names = load_level_list();
	load_level(level_names[0], player, enemies, map, health);
	spawn_pellets();
	
	hurt.loadFromFile("hit.wav");
	jump.loadFromFile("jump.wav");
	pickup.loadFromFile("pickup.wav");
	kill.loadFromFile("kill.wav");
	hurtSound.setBuffer(hurt);
	pickupSound.setBuffer(pickup);
	jumpSound.setBuffer(jump);
	killSound.setBuffer(kill);

	player_walk_tex = load_texture("walk.png");
	player_jump_tex = load_texture("jump.png");
	player_fall_tex = load_texture("fall.png");

	player_jump = sf::Sprite(player_jump_tex);
	player_fall = sf::Sprite(player_fall_tex);
	for(int i = 0; i < 6; i++) {
		player_walk.push_back(sf::Sprite(player_walk_tex, sf::IntRect(i * 32, 0, 32, 28)));
	}
}

bool Game::tick(sf::RenderWindow &window) {
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
		edit(window, justPressedE); 
	} else {
		//Update the game
		update() ;
	}
	//Render the game
	render_state(window);

	if(pellets.empty()) {
		level_index++;
		if(level_index >= level_names.size()) {
			load_level(level_names[level_index], player, enemies, map, health);
			spawn_pellets();
		} else {
			scoreDisplay.setString("You Win!");
		}
	}

	if(health == 0) {
		load_level(level_names[level_index], player, enemies, map, health);
	}
	music.update();
	return false;
}

void Game::spawn_pellets() {
	for(int i = 0; i < map.getWidth(); i += 32) 
		for(int j = 0; j < map.getHeight(); j += 32) 
			if(map.free(i, j) && rand() < RAND_MAX / 7)
				pellets.push_back(Object(sf::Color::White, i + 16, j + 16, 6, 0.5f));
}

void Game::edit(sf::Window &window, bool justPressedE) {
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

void Game::update() {
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
		jumpSound.play();
	}
	//Update all of the pellets
	for(auto pellet = pellets.begin(); pellet < pellets.end(); pellet++) {
		if(pellet->collides(player)) {
			pickupSound.play();
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
	for(auto enemy = enemies.begin(); enemy < enemies.end(); enemy++) {
		if(abs(enemy->x - player.x) < 2) {
			enemy->x = player.x;
			enemy->xspeed = 0;
		} else if(enemy->x < player.x) {
			enemy->xspeed = 0.75;
		} else if(enemy->x > player.x) {
			enemy->xspeed = -0.75;
		}
		if(enemy->yspeed < 8 && enemy->y > player.y) {
			enemy->yspeed = -2;
		}
		enemy->fall(map, 0.25);
		if(enemy->collides(player)) {
			if(player.y < enemy->y - enemy->radius / 2) {
				killSound.play();
				enemy = enemies.erase(enemy);
			} else if(hurt_cooldown == 0) {
				hurt_cooldown = max_hurt_cooldown;
				health -= 1;
				hurtSound.play();
			}
		}
	}
	if(hurt_cooldown > 0) hurt_cooldown--;
}

void Game::render_state(sf::RenderWindow &window) {
	window.clear();
	sf::View view(sf::FloatRect(0, 40, game_width, game_height));
	view.setViewport(sf::FloatRect(0, 40.0f / window_height, 1, 1));
	window.setView(view);
	for(int x = 0; x < map.getWidth(); x += 32) {
		for(int y = 0; y < map.getHeight(); y += 32) {
			if(map.get(x, y)) {
				sf::RectangleShape rect(sf::Vector2f(32, 32));
				rect.setPosition(x, y);
				rect.setFillColor(sf::Color::Magenta);
				window.draw(rect);
			}
		}
	}
	for(auto pellet : pellets)
		pellet.draw(window);
	for(auto ghost : ghostPellets)
		ghost.draw(window);
	for(auto enemy : enemies)
		enemy.draw(window);
	sf::Sprite player_sprite;
	if(player.yspeed == 0) {
		player_sprite = player_walk[player_frames / 6];
		if(player.xspeed != 0) {
			player_frames = (player_frames + 1) % (player_walk.size() * 6);
		}
	} else if(player.yspeed < 0) {
		player_sprite = player_jump;
	} else {
		player_sprite = player_fall;
	}
	player_sprite.setOrigin(13, 20);
	player_sprite.setPosition(player.x, player.y);
	player_sprite.setScale(-2 * player.flip_x + 1, 1);
	window.draw(player_sprite);
	window.setView(window.getDefaultView());
	window.draw(scoreDisplay);
	for(int i = 0; i < health; i++) {
		sf::CircleShape circ(8);
		circ.setPosition(i * 20 + 16, 16);
		circ.setFillColor(sf::Color::Cyan);
		window.draw(circ);
	}
	window.display();
}
