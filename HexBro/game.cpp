#include "game.h"
#include "util.h"
#include<iostream>
#include<fstream>
#include<sstream>
#include <ctime>
#include<chrono>
#include<string>
#include<iomanip>
using namespace std;

static int u[] = { 0,0,1,-1,1,-1 };
static int v[] = { 1,-1,0,0,1,-1 };
//static bool check_map[BOARD_SIZE][BOARD_SIZE];

void Board::do_move(Board& board, int move) {
	board.board[move] = board.run_player;
	board.step[board.step_sum] = move;
	board.step_sum++;
	board.run_player *= -1;
}

void Board::print(Board& board) {
	int(*board2d)[BOARD_SIZE] = (int(*)[BOARD_SIZE])board.board;
	int z_y = BOARD_SIZE;
	char z_x = 'A';
	for (int y = BOARD_SIZE - 1; y > -1; y--) {
		for (int i = 0; i < BOARD_SIZE - y - 1; i++) cout << ' ';
		cout << setw(3) << z_y-- << ' ';
		for (int x = 0; x < BOARD_SIZE; x++) {
			switch (board2d[x][y]) {
			case 0:cout << '.'; break;
			case 1:cout << 'o'; break;
			case -1:cout << 'x'; break;
			}
			cout << ' ';
		}
		cout << endl;
	}
	for (int i = 0; i < BOARD_SIZE + 3; i++)cout << ' ';
	for (int i = 0; i < BOARD_SIZE; i++)cout << ' ' << z_x++;
	cout << endl;

	if (board.step_sum > 0) {
		char x;
		int y;
		move_to_number(board.step[board.step_sum - 1], x, y);
		if (board.run_player == -1) {
			cout << "last move: o" << '(' << x << ',' << y << ')' << endl;
		}
		else if (board.run_player == 1) {
			cout << "last move: x" << '(' << x << ',' << y << ')' << endl;
		}
	}
	if (board.state == 1) {
		cout << "red player win!" << endl;
	}
	if (board.state == -1) {
		cout << "blue player win!" << endl;
	}
}

int Board::location_to_move(int x, int y) {
	return x * BOARD_SIZE + y;
}

void Board::move_to_location(int move, int& x, int& y) {
	x = move / BOARD_SIZE;
	y = move % BOARD_SIZE;
}

int Board::number_to_move(char x, int y) {
	if (x >= 'a') {
		x -= 32;
	}
	int l_x = x - 'A';
	int l_y = y - 1;
	if (l_x >= 0 && l_x < BOARD_SIZE && l_y >= 0 && l_y < BOARD_SIZE) {
		return location_to_move(l_x, l_y);
	}
	return -1;
}

void Board::move_to_number(int move, char& x, int& y) {
	int l_x, l_y;
	move_to_location(move, l_x, l_y);
	x = 'A' + l_x;
	y = l_y + 1;
}

void Board::random_do_move(Board& board) {
	int r = rand() % (BOARD_SIZE * BOARD_SIZE - board.step_sum);
	int move = -1;
	do {
		move++;
		while (board.board[move]) {
			move++;
		}
	} while (r--);
	do_move(board, move);
}

void Board::call_back(Board& board) {
	if (board.step_sum > 0) {
		board.step_sum--;
		board.board[board.step[board.step_sum]] = 0;
		board.run_player = ((board.step_sum + 1) % 2) * 2 - 1;
		board.state = 0;
	}
}

void Board::redo(Board& board) {
	board.board[board.step[board.step_sum]] = board.run_player;
	board.step_sum++;
	board.run_player = ((board.step_sum + 1) % 2) * 2 - 1;
}

vector<int> Board::get_available_moves(Board& board) {
	vector<int> available_moves;
	for (int move = 0; move < BOARD_SIZE * BOARD_SIZE; move++) {
		if (board.board[move] == 0) {
			available_moves.push_back(move);
		}
	}

	return available_moves;
}

int Board::check_state(Board& _board) {
	if (_board.step_sum < BOARD_SIZE * 2 - 1) return 0;
	Board board = _board;

	int(*board2d)[BOARD_SIZE] = (int(*)[BOARD_SIZE])board.board;
	int x, y, x0, y0;

	queue<int> search_queue_x;
	queue<int> search_queue_y;

	for (int i = 0; i < BOARD_SIZE; i++) {
		if (board2d[i][0] == 1) {
			search_queue_x.push(i);
			search_queue_y.push(0);
			board2d[i][0] = 0;
			while (!search_queue_x.empty()) {
				x = search_queue_x.front();
				y = search_queue_y.front();
				search_queue_x.pop();
				search_queue_y.pop();
				if (y == BOARD_SIZE - 1) {
					_board.state = 1;
					return 1;
				}
				for (int j = 0; j < 6; j++) {
					x0 = x + u[j];
					y0 = y + v[j];
					if (-1 < x0 && x0 < BOARD_SIZE && -1 < y0 && y0 < BOARD_SIZE && board2d[x0][y0] == 1) {
						search_queue_x.push(x0);
						search_queue_y.push(y0);
						board2d[x0][y0] = 0;
					}
				}
			}
		}
	}

	for (int i = 0; i < BOARD_SIZE; i++) {
		if (board2d[0][i] == -1) {
			search_queue_x.push(0);
			search_queue_y.push(i);
			board2d[0][i] = 0;
			while (!search_queue_x.empty()) {
				x = search_queue_x.front();
				y = search_queue_y.front();
				search_queue_x.pop();
				search_queue_y.pop();
				if (x == BOARD_SIZE - 1) {
					_board.state = -1;
					return -1;
				}
				for (int j = 0; j < 6; j++) {
					x0 = x + u[j];
					y0 = y + v[j];
					if (-1 < x0 && x0 < BOARD_SIZE && -1 < y0 && y0 < BOARD_SIZE && board2d[x0][y0] == -1) {
						search_queue_x.push(x0);
						search_queue_y.push(y0);
						board2d[x0][y0] = 0;
					}
				}
			}
		}
	}

	return 0;
}

Player::Player(string name) {
	this->name = name;
}
void Player::game_start_fn(Board::Board* board) {
	this->board = board;
}

int Player::get_move_fn() {
	vector<int> available_moves = Board::get_available_moves(*this->board);
	return available_moves[rand() % available_moves.size()];
}
void Player::call_back_fn() {

}
Game::Game(Board::Board* board) {
	this->board = board == NULL ? new Board::Board : board;
}
Game::~Game() {
	delete this->board;
}
void Game::bind_players(Player* player1, Player* player2) {
	this->player1 = player1;
	this->player2 = player2;
	this->player1->game_start_fn(this->board);
	this->player2->game_start_fn(this->board);
	this->game_id = this->player1->name + this->player2->name;
	if (recovery::check_recovery_file(this->game_id)) {
		this->recovery();
		cout << "发现棋局恢复文件，已恢复棋局!" << endl;
	}
}
void Game::start() {
	int c = 0;
	time_t start = 0, l;
	while (true) {
		start = time(NULL);
		cout << "----------------------------------------" << endl;
		Board::print(*this->board);
		c = this->pause_settings();
		if (c == 0) {
			this->game_step();
			l = time(NULL) - start;
			if (this->board->run_player == 1) {
				this->p2 += l;
			}
			else {
				this->p1 += l;
			}
		}
		cout << "p1:" << this->p1 << "s\tp2:" << this->p2 << "s" << endl;
		if (c == -1) {
			this->call_back();
		}
		if (c == -2) {
			this->redo();
		}
		if (c == 1) {
			this->output_sheet();
		}
	}
}

int Game::pause_settings() {
	if (this->board->state == 0)cout << "继续-fd" << endl;
	if (this->board->step_sum > 0)cout << "撤回-cb" << endl;
	if (this->board->step_sum < this->max_step)cout << "重做-rd" << endl;
	cout << "保存棋谱-save" << endl;
	string str;
	cout << "输入操作: ";
	cin.ignore();
	cin >> str;
	while (true) {
		if (this->board->state == 0 && str == "fd")return 0;
		if (this->board->step_sum > 0 && str == "cb")return -1;
		if (this->board->step_sum < this->max_step && str == "rd")return -2;
		if (str == "save")return 1;
		cout << "输入错误, 请重新输入: ";
		cin.ignore();
		cin >> str;
	}
	return 0;
}
void Game::game_step() {
	if (this->board->run_player == 1) {
		this->run_player = this->player1;
	}
	else if (this->board->run_player == -1) {
		this->run_player = this->player2;
	}
	int move = this->run_player->get_move_fn();
	Board::do_move(*this->board, move);
	recovery::write_step(this->game_id, move);
	Board::check_state(*this->board);
	this->max_step = this->board->step_sum;
}
void Game::call_back() {
	if (this->board->step_sum > 0) {
		Board::call_back(*this->board);
		this->player1->call_back_fn();
		this->player2->call_back_fn();
		recovery::write_step(this->game_id, -1);
	}
}

void Game::redo() {
	if (this->board->step_sum < this->max_step) {
		Board::redo(*this->board);
		Board::check_state(*this->board);
		recovery::write_step(this->game_id, -2);
	}
}

void Game::output_sheet() {
	ostringstream n_oss;
	n_oss << "HEX-" << this->player1->name << " vs " << this->player2->name << '-';
	if (this->board->state == 1) {
		n_oss << "先手胜";
	}
	else if (this->board->state == -1) {
		n_oss << "后手胜";
	}
	else {
		n_oss << "未结束";
	}
	n_oss << ".txt";

	ostringstream d_oss;
	//head1
	d_oss << "{[HEX]";

	//head2
	d_oss << '[' << this->player1->name << " R]";

	//head3
	d_oss << '[' << this->player2->name << " B]";

	//head4
	if (this->board->state == 1) {
		d_oss << "[先手胜]";
	}
	else if (this->board->state == -1) {
		d_oss << "[后手胜]";
	}
	else {
		d_oss << "[未结束]";
	}

	//head5
	auto now = std::chrono::system_clock::now();
	auto in_time_t = std::chrono::system_clock::to_time_t(now);
	std::tm buf;
#ifdef _MSC_VER
	localtime_s(&buf, &in_time_t);
#else
	localtime_r(&in_time_t, &buf);
#endif
	d_oss << std::put_time(&buf, "[%Y.%m.%d %H:%M XX]");

	//head6
	d_oss << "[XXXX XXXX]";

	char x;
	int y;
	for (int i = 0; i < this->board->step_sum; i++) {
		Board::move_to_number(this->board->step[i], x, y);
		if (i % 2 == 0) {
			d_oss << ";R(" << x << ',' << y << ')';
		}
		else {
			d_oss << ";B(" << x << ',' << y << ')';
		}
	}
	d_oss << '}';
	ofstream file;
	file.open(n_oss.str());
	file << d_oss.str();
	file.close();
}

void Game::recovery() {
	vector<int> data = recovery::read_file(this->game_id);
	for (int action : data) {
		if (action == -1) {
			Board::call_back(*this->board);
		}
		else if (action == -2) {
			Board::redo(*this->board);
		}
		else {
			Board::do_move(*this->board, action);
		}
	}
	this->max_step = this->board->step_sum;
}
