#pragma once

#include <string>
#include <vector>

#include "game-object.hpp"
#include "tilemap.hpp"

std::vector<std::string> load_level_list();
void load_level(std::string filename, Object &playerOut, std::vector<Object> &enemiesOut, Tilemap &mapOut);
void save_level(std::string filename, Object playerOut, std::vector<Object> enemies, Tilemap map);
