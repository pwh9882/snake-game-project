#ifndef _GAMEMANAGER_H_
#define _GAMEMANAGER_H_
#include <vector>
#include "stage-manager.h"
class GameManager
{
public:
    Stage *curr_stage;
    std::vector<std::vector<int>> game_map;
    int map_height;
    int map_width;

    int current_snake_length;
    int max_snake_length;

    int growth_item_count;
    int posion_item_count;
    int gate_passed_count;

    int item_spawn_cooltime_counter;

    int is_snake_passing_gate_flag = 0;
    int gate_spawn_cooltime_counter;

    int current_game_speed;
    int current_game_elapsed_time;

    bool game_over_flag;
    bool game_complete_flag;

    GameManager();
    void initGame(int stage_index);
    void updateGame();
    void tryMoveHeadTo(int next_X, int next_Y, int head_X, int head_Y);
    void endGame();
};

#endif