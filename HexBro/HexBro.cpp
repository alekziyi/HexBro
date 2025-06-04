#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>
#include "game.h"
#include "player.h"
#include <time.h>
#include "util.h"

// 函数：读取配置文件


int main()
{
    srand(time(NULL));
    int is_first = 1;
    std::string my_name;
    std::string enemy_name;

    config::read_config(is_first, my_name, enemy_name);
    MCTSPlayer* my_player = new MCTSPlayer(my_name);
    HumanPlayer* enemy_player = new HumanPlayer(enemy_name);
    Game game;
    if (is_first == 1) {
        game.bind_players(my_player, enemy_player);
    }
    else if (is_first == 0) {
        game.bind_players(enemy_player, my_player);
    }
    else {
        game.bind_players(new MCTSPlayer("p1"), new MCTSPlayer("p2"));
    }
    game.start();
}
