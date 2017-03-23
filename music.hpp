#pragma once

#include <SFML/Audio.hpp>
#include <vector>

std::vector<sf::Music> load_tracks();
void update_track(std::vector<sf::Music> tracks);
