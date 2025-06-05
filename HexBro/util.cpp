#include "util.h"
#include<fstream>
#include <regex>
#include<sstream>
#include <filesystem>
using namespace std;

void config::read_config(int& is_first, std::string& player1_name, std::string& player2_name) {
	std::string file_name = "config.ini";
	std::ifstream config(file_name);
	std::string line;

	if (!config.is_open()) {
		std::cerr << "未找到: " << file_name << std::endl;
		std::ofstream outFile(file_name);
		if (outFile.is_open()) {  // 检查文件是否成功打开
			outFile << "# 我方先后手(1:先手,0:后手)\n";  // 写入内容
			outFile << "is_first=0\n";
			outFile << "# 我方程序名\n";
			outFile << "my_name=human\n";
			outFile << "# 敌方程序名\n";
			outFile << "other_name=computer\n";
			outFile.close();  // 关闭文件
			std::cout << "已创建默认配置文件\n";
		}
		else {
			std::cerr << "Failed to open file!\n";
		}
		is_first = 0;
		player1_name = "human";
		player2_name = "computer";
		return;

	}

	while (std::getline(config, line)) {
		if (line.find("is_first=") == 0) {
			is_first = std::stoi(line.substr(line.find('=') + 1));
		}
		else if (line.find("my_name=") == 0) {
			player1_name = line.substr(line.find('=') + 1);
		}
		else if (line.find("other_name=") == 0) {
			player2_name = line.substr(line.find('=') + 1);
		}
	}

	config.close();
}


bool recovery::check_recovery_file(string game_id) {
	string file_name = game_id + "recovery.txt";
	if (std::filesystem::exists(file_name)) {
		return true;
	}
	return false;
}

vector<int> recovery::read_file(string game_id) {
	string file_name = game_id + "recovery.txt";
	ifstream recovery_file(file_name);
	std::stringstream buffer;
	buffer << recovery_file.rdbuf();
	recovery_file.close();
	cout << "read" << endl;
	std::vector<int> actions;
	std::regex reg(R"(\[(-?\d+)\])");
	string data = buffer.str();
	cout << data << endl;
	// 使用smatch来保存匹配结果
	std::smatch matches;
	// 使用regex_search来查找匹配项
	while (std::regex_search(data, matches, reg)) {
		// 将匹配到的字符串转换为整数并添加到vector中
		actions.push_back(std::stoi(matches[1].str()));
		// 更新搜索起点，跳过已经匹配的部分
		data = matches.suffix().str();
	}
	return actions;
}

void recovery::write_step(string game_id, int action) {
	string file_name = game_id + "recovery.txt";
	ofstream recovery_file(file_name, ios::app);
	recovery_file << '[' << action << ']';
	recovery_file.close();
}

void  game::get_input_to_number(char& _x, int& _y) {
	string str;
	char x;
	int y;
	std::regex reg("[A-Za-z]\\d+");
	while (true) {
		y = 0;
		cin.ignore();
		cin >> str;

		std::smatch matches;
		if (std::regex_search(str, matches, reg)) {
			string number = matches[0];
			if (number.size() == 2) {
				x = number[0];
				y = number[1] - '0';
			}
			else if (number.size() == 3) {
				x = number[0];
				y = 10 * (number[1] - '0');
				y += number[2] - '0';
			}
			else {
				std::cout << "输入有误，请重新输入: ";
				continue;
			}

			if (x >= 'a') {
				x -= 32;
			}
			if (x >= 'A' && x < ('A' + BOARD_SIZE) && y>0 && y <= BOARD_SIZE) {
				_x = x;
				_y = y;
				return;
			}
			else {
				std::cout << "输入有误，请重新输入: ";
			}
		}
		else {
			std::cout << "输入有误，请重新输入: ";
		}
	}
}