#pragma once

#include <SFML/Graphics.hpp>

#include "screen.hpp"

class StartScreen : public Screen {
public:
	StartScreen() {
		texture.loadFromFile("title-card.png");
		card.setTexture(texture);
	}

	bool tick(sf::RenderWindow &window) {
		window.clear();
		window.draw(card);
		window.display();
		return sf::Keyboard::isKeyPressed(sf::Keyboard::Space);
	}

private:
	sf::Sprite card;
	sf::Texture texture;
};
