#include "render.hpp"
#include "main.hpp"

void render_state(sf::RenderWindow &window, Tilemap map, Object player, std::vector<Object> enemies, 
		std::vector<Object> pellets, std::vector<Object> ghostPellets, sf::Text &scoreDisplay, int health) {
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
	player.draw(window);
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
