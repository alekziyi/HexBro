#pragma once
#include "game.h"
#include <string.h>
#include <map>
#include <shared_mutex>

class Node {
public:
	Node(int player, Node* parent = NULL);
	~Node();
	int visit = 0;
	int value = 0;
	short player;
	short n_access = 0;
	short n_children = 0;
	Node* parent;
	Node* children[BOARD_SIZE * BOARD_SIZE] = { 0 };
	std::mutex _mutex;
};

class MCTSEngine {
public:
	MCTSEngine();
	bool s = true;
	time_t search_time = 0;
	Node* root = NULL;
	int get_move(Board::Board* board);
	void playout(Board::Board board);
	void reset();
private:
	Node* select(Board::Board& board);
	void expand(Node* node, Board::Board& board);
	int simulate(Board::Board& board);
	void backtrack(Node* node, int state);
};
