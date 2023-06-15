#ifndef _MAPMANAGER_H_
#define _MAPMANAGER_H_
#include <vector>
class Stage
{

public:
    static const int MAX_MAP_WIDTH = 50;
    static const int MAX_MAP_HEGHIT = 21;

    int map_width;
    int map_height;

    // map configs
    int start_snake_length;
    int start_snake_speed;
    int growth_item_goal;
    int posion_item_goal;
    int gate_pass_goal;
    int item_spawn_cooltime;
    int gate_spawn_cooltime;

    std::vector<std::vector<int>> map;

    Stage(int height, int width, int newMap[MAX_MAP_HEGHIT][MAX_MAP_WIDTH], int start_snake_length) : map_width(width), map_height(height), start_snake_length(start_snake_length)
    {
        for (int i = 0; i < height; i++)
        {
            map.push_back(std::vector<int>(newMap[i], newMap[i] + width));
        }
        setStageConfigs();
    }

    void setStageConfigs()
    {
        start_snake_length = 4;
        start_snake_speed = 1;
        growth_item_goal = 5;
        posion_item_goal = 5;
        gate_pass_goal = 3;
        gate_spawn_cooltime = 10;
        item_spawn_cooltime = 5;
    }
};

class StageManager
{
public:
    StageManager();
    std::vector<Stage> stages;
};

#endif