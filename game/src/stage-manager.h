

class StageManager
{
public:
    int **stage_map;
    int map_height;
    int map_width;
    const int start_snake_length = 4;
    int current_snake_length;
    int max_snake_length;

    int growth_item_count;
    int posion_item_count;
    int gate_passed_count;

    int growth_item_goal;
    int posion_item_goal;
    int gate_pass_goal;

    int item_spawn_cooltime;
    int item_spawn_cooltime_counter;

    int is_snake_passing_gate_flag = 0;
    int gate_spawn_cooltime;
    int gate_spawn_cooltime_counter;

    int current_game_speed;
    int current_game_elapsed_time;

    bool game_over_flag;
    bool stage_complete_flag;

    StageManager();
    void initStage(int stage_index);
    void updateGame();
    void tryMoveHeadTo(int next_X, int next_Y, int head_X, int head_Y);
    void endGame();
};