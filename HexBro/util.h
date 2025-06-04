#pragma once
#include "config.h"
#include <iostream>
#include <vector>
#include <string>

namespace config {
	void read_config(int& is_first, std::string& player1_name, std::string& player2_name);
}

namespace recovery {
	bool check_recovery_file(std::string game_id);
	std::vector<int> read_file(std::string game_id);
	void write_step(std::string game_id, int action);
}

namespace game {
	void get_input_to_number(char& x, int& y);
}
