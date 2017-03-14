#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>

#include "game-object.hpp"
#include "tilemap.hpp"

void draw_object(sf::RenderWindow &window, Object obj) {
	sf::CircleShape shape(obj.radius);
	shape.setPosition(obj.x, obj.y);
	shape.setFillColor(obj.color);
	window.draw(shape);
}

int main()
{
	Tilemap map(640, 480, 32, 32);
    sf::RenderWindow window(sf::VideoMode(640, 480), "SFML works!");
	Object player(sf::Color::Cyan, 100, 100, 10);
	sf::Clock clock;

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
			player.yspeed = -16;

        window.clear();
		draw_object(window, player);
        window.display();

		sf::Time sleepTime = sf::milliseconds(16) - clock.getElapsedTime();
		if(sleepTime > sf::milliseconds(1))
			sf::sleep(sleepTime);
		clock.restart();
    }

    return 0;
}
