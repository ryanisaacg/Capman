#include "music.hpp"

#include <fstream>

const int seconds = 30;

MusicPlayer::MusicPlayer() {
	std::ifstream input("tracks");
	while(!input.eof()) {
		std::string filename;
		getline(input, filename);
		track_files.push_back(filename);
	}
	input.close();
	current.openFromFile(track_files[0]);
	current.play();
	track_index = 0;
	volume = 100;
	current.setLoop(true);
}

void MusicPlayer::update() {
	if(clock.getElapsedTime() > sf::seconds(seconds)) {
		clock.restart();
		track_index = (track_index + 1) % track_files.size();
		current.stop();
		current.openFromFile(track_files[track_index]);
		current.play();
	}
}


