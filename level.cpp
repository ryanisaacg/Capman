#include "level.hpp"

#include <fstream>

std::vector<std::string> load_level_list() {
	std::ifstream input("levels");
	std::vector<std::string> levels;
	while(!input.eof()) {
		std::string next;
		getline(input, next);
		levels.push_back(next);
	}
	return levels;
}


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
