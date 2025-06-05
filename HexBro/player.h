//文件名: player.h
//声明: Player类及其派生类HumanPlayer和MCTSPlayer
#pragma once
#include <algorithm>
#include "config.h"
#include "game.h"
#include "mcts_engine.h"
class HumanPlayer : public Player {
public:
	HumanPlayer(std::string name);
	void reset();
	int get_move_fn();
};
class MCTSPlayer : public Player {
public:
	MCTSPlayer(std::string name);
	MCTSEngine engine;
	int get_move_fn();
	void call_back_fn();
};