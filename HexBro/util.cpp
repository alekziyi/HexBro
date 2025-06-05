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
		std::cerr << "δ�ҵ�: " << file_name << std::endl;
		std::ofstream outFile(file_name);
		if (outFile.is_open()) {  // ����ļ��Ƿ�ɹ���
			outFile << "# �ҷ��Ⱥ���(1:����,0:����)\n";  // д������
			outFile << "is_first=0\n";
			outFile << "# �ҷ�������\n";
			outFile << "my_name=human\n";
			outFile << "# �з�������\n";
			outFile << "other_name=computer\n";
			outFile.close();  // �ر��ļ�
			std::cout << "�Ѵ���Ĭ�������ļ�\n";
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
	// ʹ��smatch������ƥ����
	std::smatch matches;
	// ʹ��regex_search������ƥ����
	while (std::regex_search(data, matches, reg)) {
		// ��ƥ�䵽���ַ���ת��Ϊ��������ӵ�vector��
		actions.push_back(std::stoi(matches[1].str()));
		// ����������㣬�����Ѿ�ƥ��Ĳ���
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
				std::cout << "������������������: ";
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
				std::cout << "������������������: ";
			}
		}
		else {
			std::cout << "������������������: ";
		}
	}
}