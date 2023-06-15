#ifndef _GAMEMANAGER_H_
#define _GAMEMANAGER_H_
#include <vector>
#include "stage-manager.h"
class GameManager
{
public:
    Stage *curr_stage;
    std::vector<std::vector<int>> current_game_map;
    int map_height;
    int map_width;

    int current_game_speed;
    int current_game_elapsed_time;
    int current_snake_length;
    int top_snake_length;

    int growth_item_count;
    int posion_item_count;
    int gate_passed_count;

    int item_spawn_cooltime_counter;
    int gate_spawn_cooltime_counter;

    int gate_passing_required_count;

    bool game_over_flag;
    bool game_complete_flag;

    unsigned int total_score;

    GameManager();
    void initGame(int stage_index);
    void updateGame();
    void calcTotalScore();
    void endGame();

private:
    void tryMoveHeadTo(int next_X, int next_Y, int head_X, int head_Y);
    void moveHeadToGrowth();
    void moveHeadToPosion();
    void moveHeadToGate(int next_X, int next_Y, int head_X, int head_Y);
    void moveHeadToWall();
    void moveHeadToHighWall(int next_X, int next_Y, int head_X, int head_Y);
};

#endif