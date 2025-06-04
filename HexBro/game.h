#pragma once

#include "config.h"
#include<vector>
#include<queue>
#include<string>

namespace Board {
	typedef struct _Board {
		int step_sum = 0; //�Ѿ����ӹ��Ĳ���
		int run_player = 1; //��һ�����ӵ����(1���֣�-1����)
		int board[BOARD_SIZE * BOARD_SIZE] = { 0 }; //һά����
		int step[BOARD_SIZE * BOARD_SIZE] = { 0 }; //������־
		int state = 0; //״̬(0�Ծ��� 1����ʤ -1����ʤ)
	} Board;

	void do_move(Board& board, int move); //����������
	void print(Board& board); //��ӡ����
	int location_to_move(int x, int y); //��ά����תһά����
	void move_to_location(int move, int& x, int& y); //һά����ת��ά����
	int number_to_move(char x, int y);
	void move_to_number(int move, char& x, int& y);
	void random_do_move(Board& board);
	void call_back(Board& board);
	void redo(Board& board);
	std::vector<int> get_available_moves(Board& board); //��ȡ��λһά�����б�
	int check_state(Board& _board); //�������״̬

};

class Player {
public:
	Player() :name("player"), board(NULL), winner(false) {};
	Player(std::string name);
	std::string name;
	Board::Board* board = NULL;
	bool winner = false;
	virtual void game_start_fn(Board::Board* board);
	virtual int get_move_fn();
	virtual void call_back_fn();
};

class Game
{
public:
	Game(Board::Board* board = NULL);
	~Game();
	std::string game_id;
	Player* player1 = NULL;
	Player* player2 = NULL;
	Player* run_player = NULL;
	void bind_players(Player* player1, Player* player2);
	void start();
	void output_sheet();
private:
	time_t p1 = 0;
	time_t p2 = 0;
	int pause_settings();
	void game_step();
	void call_back();
	void redo();
	int max_step = 0;
	Board::Board* board;
	void recovery();
};

