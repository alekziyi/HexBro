#include "player.h"
#include "util.h"
#include <iostream>
using namespace std;


HumanPlayer::HumanPlayer(string name) {
	this->name = name;
}
int HumanPlayer::get_move_fn() {
	char x;
	int y;
	int move;
	cout << "Please input your move:";
	game::get_input_to_number(x, y);
	move = Board::number_to_move(x, y);
	while (0 != this->board->board[move]) {
		cout << "Please retry to input your move:";
		game::get_input_to_number(x, y);
		move = Board::number_to_move(x, y);
	}
	return move;
}

void HumanPlayer::reset() {

}
//***********************************************************
MCTSPlayer::MCTSPlayer(string name) {
	this->name = name;
}

int MCTSPlayer::get_move_fn() {
	if (this->board->step_sum == 0) {
		return Board::location_to_move(5, 5);
	}
	return this->engine.get_move(this->board);
}

void MCTSPlayer::call_back_fn() {
	this->engine.reset();
}
