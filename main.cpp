#include <cmath>
#include <iostream>
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <string>
#include <vector>

#include "game-object.hpp"
#include "tilemap.hpp"

void draw_object(sf::RenderWindow &window, Object obj) {
	sf::CircleShape shape(obj.radius * obj.draw_scale);
	shape.setPosition(obj.x, obj.y);
	shape.setFillColor(obj.color);
	window.draw(shape);
}

void spawn_pellets(Tilemap map, std::vector<Object> &pellets) {
	for(int i = 0; i < map.getWidth(); i += 32) 
		for(int j = 0; j < map.getHeight(); j += 32) 
			if(map.free(i, j))
				pellets.push_back(Object(sf::Color::White, i + 16, j + 16, 6, 0.5f));
}

int main() {
	Tilemap map(640, 480, 32, 32);
	for(int i = 33; i < map.getWidth(); i++)
		map.set(100, i, 400); 
    sf::RenderWindow window(sf::VideoMode(640, 520), "SFML works!");
	Object player(sf::Color::Cyan, 100, 100, 10);
	sf::Clock clock;
	std::vector<Object> pellets, ghostPellets, enemies;
	sf::Font font;
	font.loadFromFile("arial.ttf");
	sf::Text scoreDisplay("", font);
	scoreDisplay.setOutlineColor(sf::Color::Yellow);
	scoreDisplay.setFillColor(sf::Color::White);
	scoreDisplay.setOutlineThickness(0.5);
	int score = 0;

	spawn_pellets(map, pellets);
	enemies.push_back(Object(sf::Color::Red, 200, 90, 10));

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
        }

		if(sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
			player.xspeed = -4;
		} else if(sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
			player.xspeed = 4;
		} else {
			player.xspeed = 0;
		}
		player.fall(map, 0.5);

		if(sf::Keyboard::isKeyPressed(sf::Keyboard::W) && player.supported(map))
			player.yspeed = -12;

		for(auto pellet = pellets.begin(); pellet < pellets.end(); pellet++) {
			if(pellet->collides(player)) {
				ghostPellets.push_back(Object(sf::Color::Yellow, player.x, player.y, pellet->radius * 2));
				pellet = pellets.erase(pellet);
				score++;
				scoreDisplay.setString(std::to_string(score));
				scoreDisplay.setPosition(320 - scoreDisplay.getLocalBounds().width / 2, 0);
			}
		}
		for(auto ghost = ghostPellets.begin(); ghost < ghostPellets.end(); ghost++) {
			if(ghost->radius <= 0) {
				ghost = ghostPellets.erase(ghost);
			} else {
				ghost->radius -= 0.5;
				ghost->color.a -= 10;
			}
		}
		for(auto &enemy : enemies) {
			if(abs(enemy.x - player.x) < 2) {
				enemy.x = player.x;
				enemy.xspeed = 0;
			} else if(enemy.x < player.x) {
				enemy.xspeed = 2;
			} else if(enemy.x > player.x) {
				enemy.xspeed = -2;
			}
			if(enemy.yspeed < 8 && enemy.y > player.y - 4) {
				enemy.yspeed = -4;
			}
			enemy.fall(map, 0.25);
		}
		

		window.clear();
		sf::View view(sf::FloatRect(0, 40, 640, 480));
		view.setViewport(sf::FloatRect(0, 40 / 520.0f, 1, 1));
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
			draw_object(window, pellet);
		for(auto ghost : ghostPellets)
			draw_object(window, ghost);
		for(auto enemy : enemies)
			draw_object(window, enemy);
		draw_object(window, player);
		window.setView(window.getDefaultView());
		window.draw(scoreDisplay);
		window.display();

		sf::Time sleepTime = sf::milliseconds(16) - clock.getElapsedTime();
		if(sleepTime > sf::milliseconds(1))
			sf::sleep(sleepTime);
		clock.restart();
    }

    return 0;
}
