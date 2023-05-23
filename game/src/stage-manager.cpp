#include "mangers.h"
#include <ncurses.h>
#include <random>
#include <vector>
extern InputManager inputManager;
extern MapManager mapManager;

/*
nothing: 0
snack_head:-1
snack_body:1~INT_MAX;
Immune Wall -2
Wall -3
Gate -4
Growth -15 ~ -10
Poison -25 ~ -20
*/

StageManager::StageManager()
{
}
void StageManager::initStage(int stage_index)
{

    current_game_speed = 2;
    current_game_elapsed_time = 0;

    growth_item_count = 0;
    posion_item_count = 0;
    gate_passed_count = 0;

    growth_item_goal = 5;
    posion_item_goal = 2;
    gate_pass_goal = 3;

    item_spawn_cooltime = 5;
    item_spawn_cooltime_counter = 0;

    gate_spawn_cooltime = 10;
    gate_spawn_cooltime_counter = 0;

    game_over_flag = false;
    stage_complete_flag = false;

    Map &curr_map = mapManager.maps[stage_index];
    map_height = curr_map.height;
    map_width = curr_map.width;
    stage_map = new int *[map_height];
    current_snake_length = curr_map.start_snake_length;
    for (int i = 0; i < map_height; i++)
    {
        stage_map[i] = new int[map_width];
        for (int j = 0; j < map_width; j++)
        {
            stage_map[i][j] = curr_map.data[i][j];
        }
    }
}

void StageManager::endGame()
{
    for (int i = 0; i < map_height; i++)
    {
        delete[] stage_map[i];
    }
    delete[] stage_map;
    stage_map = nullptr;
}

void StageManager::updateGame()
{
    int head_Y = 0, head_X = 0;
    // 머리 찾기
    for (int i = 0; i < map_height; i++)
    {
        for (int j = 0; j < map_width; j++)
        {
            int curr = stage_map[i][j];
            if (curr == -1)
            {
                head_Y = i;
                head_X = j;
            }
        }
        // printw("\n");
    }

    // ======================================
    // 머리 좌표 이동
    int next_Y = head_Y, next_X = head_X;
    switch (inputManager.recent_user_input)
    {
    case KEY_LEFT:
        next_X--;
        break;
    case KEY_RIGHT:
        next_X++;
        break;
    case KEY_UP:
        next_Y--;
        break;
    case KEY_DOWN:
        next_Y++;
        break;
    }

    // ======================================
    // 머리 이동 시도
    tryMoveHeadTo(next_X, next_Y, head_X, head_Y);

    // ======================================
    // 몸통죽이기 & 아이템 죽이기
    for (int i = 0; i < map_height; i++)
    {
        for (int j = 0; j < map_width; j++)
        {
            int &curr = stage_map[i][j];
            if (curr > 0)
            {
                curr--;
            }
        }
    }
    if (is_snake_passing_gate_flag > 0)
    {
        is_snake_passing_gate_flag--;
    }

    // 틱 마다 아이템 생성
    if (item_spawn_cooltime_counter == item_spawn_cooltime)
    {
        item_spawn_cooltime_counter = 0;
        std::vector<int> emptyBlockCords;
        for (int i = 0; i < map_height; i++)
        {
            for (int j = 0; j < map_width; j++)
            {

                // 아이템 죽이기
                int &curr = stage_map[i][j];
                if (curr >= -15 && curr < -10)
                {
                    curr = 0;
                }
                else if (curr >= -25 && curr < -20)
                {
                    curr = 0;
                }

                if (curr == 0)
                {
                    emptyBlockCords.push_back(i * map_width + j);
                }
            }
        }
        int index = rand() % emptyBlockCords.size();
        int value = emptyBlockCords[index];
        stage_map[value / map_width][value % map_width] = -15;

        index = rand() % emptyBlockCords.size();
        value = emptyBlockCords[index];
        stage_map[value / map_width][value % map_width] = -25;
    }

    // 틱마다 gate 생성
    if (gate_spawn_cooltime_counter > gate_spawn_cooltime && is_snake_passing_gate_flag == false)
    {
        gate_spawn_cooltime_counter = 0;
        std::vector<int> emptyWallCords;
        for (int i = 0; i < map_height; i++)
        {
            for (int j = 0; j < map_width; j++)
            {

                // 게이트 죽이기
                int &curr = stage_map[i][j];
                if (curr == -4)
                {
                    curr = -3;
                }
                if (curr == -3)
                {
                    emptyWallCords.push_back(i * map_width + j);
                }
            }
        }
        int index = rand() % emptyWallCords.size();
        int value = emptyWallCords[index];
        stage_map[value / map_width][value % map_width] = -4;

        int newIndex = rand() % emptyWallCords.size();
        while (newIndex == index) // 좌표중복방지
        {
            newIndex = rand() % emptyWallCords.size();
        }
        value = emptyWallCords[newIndex];
        stage_map[value / map_width][value % map_width] = -4;
    }
}

void StageManager::tryMoveHeadTo(int next_X, int next_Y, int head_X, int head_Y)
{
    // ======================================
    // 아무것도 없으면 이동
    if (stage_map[next_Y][next_X] == 0)
    {
        stage_map[head_Y][head_X] = current_snake_length;
        stage_map[next_Y][next_X] = -1;
    }
    // 성장아이템 먹으면 길이 + 1
    else if (stage_map[next_Y][next_X] == -15)
    {
        stage_map[head_Y][head_X] = current_snake_length++;
        stage_map[next_Y][next_X] = -1;
        if (is_snake_passing_gate_flag > 0) // 중간에 길이 변화 시 추적 기능
        {
            is_snake_passing_gate_flag++;
        }
        growth_item_count++;
        max_snake_length = (max_snake_length < current_snake_length) ? current_snake_length : max_snake_length;
        for (int i = 0; i < map_height; i++)
        {
            for (int j = 0; j < map_width; j++)
            {
                int curr = stage_map[i][j];
                if (curr > 0)
                {
                    stage_map[i][j]++;
                }
            }
        }
    }
    // posion 아이템 섭취시 몸톨 길이 -1
    else if (stage_map[next_Y][next_X] == -25)
    {
        posion_item_count++;
        stage_map[head_Y][head_X] = current_snake_length--;
        stage_map[next_Y][next_X] = -1;
        if (is_snake_passing_gate_flag > 0) // 중간에 길이 변화 시 추적 기능
        {
            is_snake_passing_gate_flag--;
        }
        for (int i = 0; i < map_height; i++)
        {
            for (int j = 0; j < map_width; j++)
            {
                int curr = stage_map[i][j];
                if (curr > 0)
                {
                    stage_map[i][j]--;
                }
            }
        }
    }
    // Gate 통과 시
    else if (stage_map[next_Y][next_X] == -4)
    {
        gate_passed_count++;
        is_snake_passing_gate_flag = current_snake_length;
        for (int k = 0; k < map_height * map_width; k++)
        {
            int i = k / map_width;
            int j = k % map_width;
            int curr = stage_map[i][j];
            if (curr == -4 && (next_Y != i || next_X != j))
            {
                next_Y = i;
                next_X = j;
                break;
            }
        }

        // 출구 정하기
        // KEY_UP 상태로 들어가기
        if (inputManager.recent_user_input == KEY_UP)
        {
            // 위 -> 오른 -> 왼 -> 아래 순으로 진출
            if (next_Y - 1 >= 0)
            {
                int curr = stage_map[next_Y - 1][next_X]; // KEY_UP
                if ((curr == 0 || curr == -15 || curr == -25))
                {
                    inputManager.recent_user_input = KEY_UP;
                    tryMoveHeadTo(next_X, next_Y - 1, head_X, head_Y);
                    return;
                }
            }
            if (next_X + 1 < map_width)
            {
                int curr = stage_map[next_Y][next_X + 1]; // KEY_RIGHT
                if ((curr == 0 || curr == -15 || curr == -25))
                {
                    inputManager.recent_user_input = KEY_RIGHT;
                    tryMoveHeadTo(next_X + 1, next_Y, head_X, head_Y);
                    return;
                }
            }
            if (next_X - 1 >= 0)
            {
                int curr = stage_map[next_Y][next_X - 1]; // KEY_LEFT
                if ((curr == 0 || curr == -15 || curr == -25))
                {
                    inputManager.recent_user_input = KEY_LEFT;
                    tryMoveHeadTo(next_X - 1, next_Y, head_X, head_Y);
                    return;
                }
            }
            if (next_Y + 1 < map_height)
            {
                int curr = stage_map[next_Y + 1][next_X]; // KEY_DOWN
                if ((curr == 0 || curr == -15 || curr == -25))
                {
                    inputManager.recent_user_input = KEY_DOWN;
                    tryMoveHeadTo(next_X, next_Y + 1, head_X, head_Y);
                    return;
                }
            }
        }

        // KEY_DOWN
        else if (inputManager.recent_user_input == KEY_DOWN)
        {
            // 아래 -> 왼 -> 위 -> 오른 순으로 진출
            if (next_Y + 1 < map_height)
            {
                int curr = stage_map[next_Y + 1][next_X]; // KEY_DOWN
                if ((curr == 0 || curr == -15 || curr == -25))
                {
                    inputManager.recent_user_input = KEY_DOWN;
                    tryMoveHeadTo(next_X, next_Y + 1, head_X, head_Y);
                    return;
                }
            }
            if (next_X - 1 >= 0)
            {
                int curr = stage_map[next_Y][next_X - 1]; // KEY_LEFT
                if ((curr == 0 || curr == -15 || curr == -25))
                {
                    inputManager.recent_user_input = KEY_LEFT;
                    tryMoveHeadTo(next_X - 1, next_Y, head_X, head_Y);
                    return;
                }
            }
            if (next_Y - 1 >= 0)
            {
                int curr = stage_map[next_Y - 1][next_X]; // KEY_UP
                if ((curr == 0 || curr == -15 || curr == -25))
                {
                    inputManager.recent_user_input = KEY_UP;
                    tryMoveHeadTo(next_X, next_Y - 1, head_X, head_Y);
                    return;
                }
            }
            if (next_X + 1 < map_width)
            {
                int curr = stage_map[next_Y][next_X + 1]; // KEY_RIGHT
                if ((curr == 0 || curr == -15 || curr == -25))
                {
                    inputManager.recent_user_input = KEY_RIGHT;
                    tryMoveHeadTo(next_X + 1, next_Y, head_X, head_Y);
                    return;
                }
            }
        }

        // => KEY_RIGHT
        else if (inputManager.recent_user_input == KEY_RIGHT)
        {
            // 오른 -> 아래 -> 위 -> 왼 순으로 진출
            if (next_X + 1 < map_width)
            {
                int curr = stage_map[next_Y][next_X + 1]; // KEY_RIGHT
                if ((curr == 0 || curr == -15 || curr == -25))
                {
                    inputManager.recent_user_input = KEY_RIGHT;
                    tryMoveHeadTo(next_X + 1, next_Y, head_X, head_Y);
                    return;
                }
            }
            if (next_Y + 1 < map_height)
            {
                int curr = stage_map[next_Y + 1][next_X]; // KEY_DOWN
                if ((curr == 0 || curr == -15 || curr == -25))
                {
                    inputManager.recent_user_input = KEY_DOWN;
                    tryMoveHeadTo(next_X, next_Y + 1, head_X, head_Y);
                    return;
                }
            }
            if (next_Y - 1 >= 0)
            {
                int curr = stage_map[next_Y - 1][next_X]; // KEY_UP
                if ((curr == 0 || curr == -15 || curr == -25))
                {
                    inputManager.recent_user_input = KEY_UP;
                    tryMoveHeadTo(next_X, next_Y - 1, head_X, head_Y);
                    return;
                }
            }
            if (next_X - 1 >= 0)
            {
                int curr = stage_map[next_Y][next_X - 1]; // KEY_LEFT
                if ((curr == 0 || curr == -15 || curr == -25))
                {
                    inputManager.recent_user_input = KEY_LEFT;
                    tryMoveHeadTo(next_X - 1, next_Y, head_X, head_Y);
                    return;
                }
            }
        }

        // <= KEY_LEFT
        else if (inputManager.recent_user_input == KEY_LEFT)
        {
            // 왼 -> 위 -> 아래 -> 위 순으로 진출
            if (next_X - 1 >= 0)
            {
                int curr = stage_map[next_Y][next_X - 1]; // KEY_LEFT
                if ((curr == 0 || curr == -15 || curr == -25))
                {
                    inputManager.recent_user_input = KEY_LEFT;
                    tryMoveHeadTo(next_X - 1, next_Y, head_X, head_Y);
                    return;
                }
            }
            if (next_Y - 1 >= 0)
            {
                int curr = stage_map[next_Y - 1][next_X]; // KEY_UP
                if ((curr == 0 || curr == -15 || curr == -25))
                {
                    inputManager.recent_user_input = KEY_UP;
                    tryMoveHeadTo(next_X, next_Y - 1, head_X, head_Y);
                    return;
                }
            }
            if (next_Y + 1 < map_height)
            {
                int curr = stage_map[next_Y + 1][next_X]; // KEY_DOWN
                if ((curr == 0 || curr == -15 || curr == -25))
                {
                    inputManager.recent_user_input = KEY_DOWN;
                    tryMoveHeadTo(next_X, next_Y + 1, head_X, head_Y);
                    return;
                }
            }
            if (next_X + 1 < map_width)
            {
                int curr = stage_map[next_Y][next_X + 1]; // KEY_RIGHT
                if ((curr == 0 || curr == -15 || curr == -25))
                {
                    inputManager.recent_user_input = KEY_RIGHT;
                    tryMoveHeadTo(next_X + 1, next_Y, head_X, head_Y);
                    return;
                }
            }
        }
    }
    else
    { // 이동 불가능한 곳으로 이동하려 했으므로 게임 오버
        game_over_flag = true;
    }
}