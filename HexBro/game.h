#pragma once

#include "config.h"
#include<vector>
#include<queue>
#include<string>

namespace Board {
	typedef struct _Board {
		int step_sum = 0; //已经走子过的步数
		int run_player = 1; //下一个走子的玩家(1先手，-1后手)
		int board[BOARD_SIZE * BOARD_SIZE] = { 0 }; //一维棋盘
		int step[BOARD_SIZE * BOARD_SIZE] = { 0 }; //走子日志
		int state = 0; //状态(0对局中 1先手胜 -1后手胜)
	} Board;

	void do_move(Board& board, int move); //对棋盘走子
	void print(Board& board); //打印棋盘
	int location_to_move(int x, int y); //二维坐标转一维坐标
	void move_to_location(int move, int& x, int& y); //一维坐标转二维坐标
	int number_to_move(char x, int y);
	void move_to_number(int move, char& x, int& y);
	void random_do_move(Board& board);
	void call_back(Board& board);
	void redo(Board& board);
	std::vector<int> get_available_moves(Board& board); //获取空位一维坐标列表
	int check_state(Board& _board); //检查棋盘状态

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

