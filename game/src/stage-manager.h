

class StageManager
{
public:
    int root_map[21][21] = {
        {-2, -3, -3, -3, -3, -3, -3, -3, -3, -3, -3, -3, -3, -3, -3, -3, -3, -3, -3, -3, -2},
        {-3, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, -3},
        {-3, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, -3},
        {-3, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, -3},
        {-3, 00, 00, -3, 00, 00, 00, 00, 00, 00, 00, -25, 00, 00, 00, 00, 00, 00, 00, 00, -3},
        {-3, 00, 00, -3, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, -3},
        {-4, 00, 00, -3, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, -3},
        {-3, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, -3},
        {-3, 00, 00, 00, 00, -15, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, -3},
        {-4, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, -3},
        {-3, 00, 00, 00, 00, 00, 00, 00, 00, -1, 03, 02, 01, 00, 00, 00, 00, 00, 00, 00, -3},
        {-3, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, -3},
        {-3, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, -3},
        {-3, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, -15, 00, 00, 00, 00, 00, 00, -3},
        {-3, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, -3},
        {-3, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, -3},
        {-3, 00, 00, 00, 00, 00, 00, 00, 00, 00, -3, 00, 00, 00, 00, 00, 00, 00, 00, 00, -3},
        {-3, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, -3},
        {-3, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, -3},
        {-3, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, -3},
        {-2, -3, -3, -3, -3, -3, -3, -3, -3, -3, -3, -3, -3, -3, -3, -3, -3, -3, -3, -3, -2},
    };
    const int start_snack_length = 4;
    int current_snack_length;
    int max_snack_length;

    int growth_item_count;
    int posion_item_count;
    int item_spawn_cooltime;
    int item_spawn_cooltime_counter;

    int gate_passed_count;
    int is_snake_passing_gate_flag = 0;
    int gate_spawn_cooltime;
    int gate_spawn_cooltime_counter;

    int current_game_speed;

    bool game_over_flag;

    StageManager();
    void initStage();
    void updateGame();
    void tryMoveHeadTo(int next_X, int next_Y, int head_X, int head_Y);
};