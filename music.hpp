#pragma once

#include <SFML/Audio.hpp>
#include <string>
#include <vector>

class MusicPlayer {
public:
	MusicPlayer();
	void update();
private:
	sf::Clock clock;
	std::vector<std::string> track_files;
	sf::Music current;
	unsigned int track_index;
	float volume;
};
