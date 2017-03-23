#include "music.hpp"

#include <fstream>


std::vector<sf::Music> load_tracks() {
	std::ifstream input("tracks");
	std::vector<sf::Music> tracks;
	while(!input.eof()) {
		std::string filename;
		getline(input, filename);
		sf::Music track;
		track.openFromFile(filename);
		tracks.push_back(track);
	}
	return tracks;
}

void update_track(std::vector<sf::Music> tracks) {
	static unsigned int track_index = 0;
	static unsigned int loops = 5;
	static float fade_out = 0;
}
