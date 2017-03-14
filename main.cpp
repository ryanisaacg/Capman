#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
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

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }

		player.fall(map, 1);

        window.clear();
		draw_object(window, player);
        window.display();

		sf::sleep(sf::milliseconds(16));
    }

    return 0;
}
