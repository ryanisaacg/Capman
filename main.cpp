#include <iostream>
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <vector>

#include "game-object.hpp"
#include "tilemap.hpp"

void draw_object(sf::RenderWindow &window, Object obj) {
	sf::CircleShape shape(obj.radius);
	shape.setPosition(obj.x, obj.y);
	shape.setFillColor(obj.color);
	window.draw(shape);
}

void spawn_pellets(Tilemap map, std::vector<Object> &pellets) {
	for(int i = 0; i < map.getWidth(); i += 32) 
		for(int j = 0; j < map.getHeight(); j += 32) 
			if(map.free(i, j)) 
				pellets.push_back(Object(sf::Color::White, i + 16, j + 16, 3));
}

int main()
{
	Tilemap map(640, 480, 32, 32);
    sf::RenderWindow window(sf::VideoMode(640, 480), "SFML works!");
	Object player(sf::Color::Cyan, 100, 100, 10);
	sf::Clock clock;
	std::vector<Object> pellets;

	spawn_pellets(map, pellets);

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }

		if(sf::Keyboard::isKeyPressed(sf::Keyboard::A))
			player.xspeed = -4;
		else if(sf::Keyboard::isKeyPressed(sf::Keyboard::D))
			player.xspeed = 4;
		else
			player.xspeed = 0;
		
		player.fall(map, 0.5);

		if(sf::Keyboard::isKeyPressed(sf::Keyboard::W) && player.supported(map))
			player.yspeed = -12;

		for(auto pellet = pellets.begin(); pellet < pellets.end(); pellet++) {
			if(pellet->collides(player)) {
				pellet = pellets.erase(pellet);
			}
		}

        window.clear();
		for(auto pellet : pellets)
			draw_object(window, pellet);
		draw_object(window, player);
        window.display();

		sf::Time sleepTime = sf::milliseconds(16) - clock.getElapsedTime();
		if(sleepTime > sf::milliseconds(1))
			sf::sleep(sleepTime);
		clock.restart();
    }

    return 0;
}
