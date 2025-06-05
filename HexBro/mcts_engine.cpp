//文件名: mcts_engine.cpp
//实现: MCTS引擎搜索
#include "mcts_engine.h"
#include <math.h>
#include <vector>
#include <iostream>
#include <thread>
#include <future>
#include <memory>
using namespace std;
static Node* null = new Node(0);
static double c = 0.9;
double UCB(Node* root, Node* node) {
	if (node == null) {
		return 10000;
	}
	return 0.5 * node->player * node->value / node->visit + 0.5 + c * sqrt(log(node->parent->visit + node->parent->n_access) / (node->visit + node->n_access));
}
Node::Node(int player, Node* parent) {
	this->parent = parent;
	this->player = player;
}
Node::~Node() {
	for (int move = 0; move < BOARD_SIZE * BOARD_SIZE; move++) {
		if (this->children[move] != NULL && this->children[move] != null) {
			delete this->children[move];
		}
	}
}
MCTSEngine::MCTSEngine() {

}
Node* MCTSEngine::select(Board::Board& board) {
	Node* p = this->root;
	Node* best_node = p;
	double best_ucb, ucb;
	int best_move = -1;
	while (p->n_children) {
		best_ucb = -100000;
		p->_mutex.lock();
		p->n_access++;
		for (int move = 0; move < BOARD_SIZE * BOARD_SIZE; move++) {
			if (p->children[move]) {
				ucb = UCB(this->root, p->children[move]);
				if (best_ucb < ucb) {
					best_ucb = ucb;
					best_move = move;
					best_node = p->children[move];
				}
			}
		}
		Board::do_move(board, best_move);
		if (best_node == null) {
			best_node = new Node(-1 * p->player, p);
			p->children[best_move] = best_node;
		}
		p->_mutex.unlock();
		p = best_node;
	}
	return best_node;
}
void MCTSEngine::expand(Node* node, Board::Board& board) {
	vector<int> available_moves = Board::get_available_moves(board);
	node->_mutex.lock();
	for (int move : available_moves) {
		node->children[move] = null;
	}
	node->n_children = available_moves.size();
	node->_mutex.unlock();
}
int MCTSEngine::simulate(Board::Board& board) {
	while (board.step_sum < BOARD_SIZE * BOARD_SIZE) {
		Board::random_do_move(board);
	}
	return Board::check_state(board);
}
void MCTSEngine::backtrack(Node* node, int state) {
	while (node) {
		node->_mutex.lock();
		node->value += state;
		node->visit++;
		node->n_access--;
		node->_mutex.unlock();
		node = node->parent;
	}
}
void MCTSEngine::playout(Board::Board board) {
	Node* node = this->select(board);
	int state = Board::check_state(board);
	if (state == 0) {
		this->expand(node, board);
		state = this->simulate(board);
	}
	this->backtrack(node, state);
}
void update_tree(Node*& root, int move) {
	Node* p = root;
	root = p->children[move];
	root->parent = NULL;
	p->children[move] = NULL;
	std::thread t([](Node* p) {delete p; }, p);
	t.detach();
}
time_t get_search_time(short step_sum, double v) {
	if (step_sum < 10)return 35;
	if (v < -0.1)return 40;
	if (v < -0.6)return 45;
	if (v > 0.7)return 15;
	return 25;
}
int MCTSEngine::get_move(Board::Board* board) {
	if (this->root == NULL) {
		this->root = new Node(board->run_player * -1);
	}
	else {
		update_tree(this->root, board->step[board->step_sum - 1]);
	}
	this->search_time += get_search_time(board->step_sum, board->run_player * this->root->value / (this->root->visit + 0.01));
	const auto search_fn = [](MCTSEngine* engine, Board::Board* board) {
		while (engine->s) {
			engine->playout(*board);
		}
		};
	vector<thread> thread_pool(THREAD_NUM);
	this->s = true;
	for (int i = 0; i < THREAD_NUM; i++) {
		thread_pool[i] = thread(search_fn, this, board);
	}
	time_t start = time(NULL);

	while (time(NULL) - start < this->search_time) {
		std::this_thread::sleep_for(std::chrono::milliseconds(100));
		printf("\r%5llds%10d%10.5f", time(NULL) - start, this->root->visit, 1.0 * this->root->value / this->root->visit);
	}
	cout << endl;
	this->s = false;
	for (int i = 0; i < THREAD_NUM; i++) {
		thread_pool[i].join();
	}
	double max_p = -100, p;
	int best_move = -1;
	for (int move = 0; move < BOARD_SIZE * BOARD_SIZE; move++) {
		if (this->root->children[move]) {
			p = this->root->children[move]->visit;
			if (max_p < p) {
				max_p = p;
				best_move = move;
			}
		}
	}
	update_tree(this->root, best_move);
	this->search_time = 0;
	return best_move;
}
void MCTSEngine::reset() {
	delete this->root;
	this->root = NULL;
	this->search_time = 15;
}