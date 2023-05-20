#include "mangers.h"
#include <ncurses.h>
#include <random>
#include <vector>
extern InputManager inputManager;

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
    initStage();
}
void StageManager::initStage()
{
    current_snack_length = start_snack_length;
    max_snack_length = current_snack_length;
    current_game_speed = 2;

    growth_item_count = 0;
    posion_item_count = 0;
    gate_passed_count = 0;

    item_spawn_cooltime = 5;
    item_spawn_cooltime_counter = 0;
}
void StageManager::updateGame()
{
    int head_Y = 0, head_X = 0;
    // 머리 찾기
    for (int i = 0; i < 21; i++)
    {
        for (int j = 0; j < 21; j++)
        {
            int curr = root_map[i][j];
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
    for (int i = 0; i < 21; i++)
    {
        for (int j = 0; j < 21; j++)
        {
            int &curr = root_map[i][j];
            if (curr > 0)
            {
                curr--;
            }
        }
    }

    // 틱 마다 아이템 생성
    if (item_spawn_cooltime_counter == 5)
    {
        item_spawn_cooltime_counter = 0;
        std::vector<int> emptyBlockCords;
        for (int i = 0; i < 21; i++)
        {
            for (int j = 0; j < 21; j++)
            {

                // 아이템 죽이기
                int &curr = root_map[i][j];
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
                    emptyBlockCords.push_back(i * 21 + j);
                }
            }
        }
        int index = rand() % emptyBlockCords.size();
        int value = emptyBlockCords[index];
        root_map[value / 21][value % 21] = -15;

        index = rand() % emptyBlockCords.size();
        value = emptyBlockCords[index];
        root_map[value / 21][value % 21] = -25;
    }
}

void StageManager::tryMoveHeadTo(int next_X, int next_Y, int head_X, int head_Y)
{
    // ======================================
    // 아무것도 없으면 이동
    if (root_map[next_Y][next_X] == 0)
    {
        root_map[head_Y][head_X] = current_snack_length;
        root_map[next_Y][next_X] = -1;
    }
    // 성장아이템 먹으면 길이 + 1
    else if (root_map[next_Y][next_X] == -15)
    {
        root_map[head_Y][head_X] = current_snack_length++;
        root_map[next_Y][next_X] = -1;
        growth_item_count++;
        max_snack_length = (max_snack_length < current_snack_length) ? current_snack_length : max_snack_length;
        for (int i = 0; i < 21; i++)
        {
            for (int j = 0; j < 21; j++)
            {
                int curr = root_map[i][j];
                if (curr > 0)
                {
                    root_map[i][j]++;
                }
            }
        }
    }
    // posion 아이템 섭취시 몸톨 길이 -1
    else if (root_map[next_Y][next_X] == -25)
    {
        posion_item_count++;
        root_map[head_Y][head_X] = current_snack_length--;
        root_map[next_Y][next_X] = -1;
        for (int i = 0; i < 21; i++)
        {
            for (int j = 0; j < 21; j++)
            {
                int curr = root_map[i][j];
                if (curr > 0)
                {
                    root_map[i][j]--;
                }
            }
        }
    }
    // Gate 통과 시
    else if (root_map[next_Y][next_X] == -4)
    {
        gate_passed_count++;
        for (int k = 0; k < 21 * 21; k++)
        {
            int i = k / 21;
            int j = k % 21;
            int &curr = root_map[i][j];
            if (curr == -4 && next_Y != i && next_X != j)
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
            int curr = root_map[next_Y - 1][next_X]; // KEY_UP
            if ((curr == 0 || curr == -15 || curr == -25) && next_Y - 1 >= 0)
            {
                inputManager.recent_user_input = KEY_UP;
                tryMoveHeadTo(next_X, next_Y - 1, head_X, head_Y);
                return;
            }
            curr = root_map[next_Y][next_X + 1]; // KEY_RIGHT
            if ((curr == 0 || curr == -15 || curr == -25) && next_X + 1 < 21)
            {
                inputManager.recent_user_input = KEY_RIGHT;
                tryMoveHeadTo(next_X + 1, next_Y, head_X, head_Y);
                return;
            }
            curr = root_map[next_Y][next_X - 1]; // KEY_LEFT
            if ((curr == 0 || curr == -15 || curr == -25) && next_X - 1 >= 0)
            {
                inputManager.recent_user_input = KEY_LEFT;
                tryMoveHeadTo(next_X - 1, next_Y, head_X, head_Y);
                return;
            }
            curr = root_map[next_Y + 1][next_X]; // KEY_DOWN
            if ((curr == 0 || curr == -15 || curr == -25) && next_Y + 1 < 21)
            {
                inputManager.recent_user_input = KEY_DOWN;
                tryMoveHeadTo(next_X, next_Y + 1, head_X, head_Y);
                return;
            }
        }

        // KEY_DOWN
        else if (inputManager.recent_user_input == KEY_DOWN)
        {
            // 아래 -> 왼 -> 위 -> 오른 순으로 진출
            int curr = root_map[next_Y + 1][next_X]; // KEY_DOWN
            if ((curr == 0 || curr == -15 || curr == -25) && next_Y + 1 < 21)
            {
                inputManager.recent_user_input = KEY_DOWN;
                tryMoveHeadTo(next_X, next_Y + 1, head_X, head_Y);
                return;
            }
            curr = root_map[next_Y][next_X - 1]; // KEY_LEFT
            if ((curr == 0 || curr == -15 || curr == -25) && next_X - 1 >= 0)
            {
                inputManager.recent_user_input = KEY_LEFT;
                tryMoveHeadTo(next_X - 1, next_Y, head_X, head_Y);
                return;
            }
            curr = root_map[next_Y - 1][next_X]; // KEY_UP
            if ((curr == 0 || curr == -15 || curr == -25) && next_Y - 1 >= 0)
            {
                inputManager.recent_user_input = KEY_UP;
                tryMoveHeadTo(next_X, next_Y - 1, head_X, head_Y);
                return;
            }
            curr = root_map[next_Y][next_X + 1]; // KEY_RIGHT
            if ((curr == 0 || curr == -15 || curr == -25) && next_X + 1 < 21)
            {
                inputManager.recent_user_input = KEY_RIGHT;
                tryMoveHeadTo(next_X + 1, next_Y, head_X, head_Y);
                return;
            }
        }

        // => KEY_RIGHT
        else if (inputManager.recent_user_input == KEY_RIGHT)
        {
            // 오른 -> 아래 -> 위 -> 왼 순으로 진출
            int curr = root_map[next_Y][next_X + 1]; // KEY_RIGHT
            if ((curr == 0 || curr == -15 || curr == -25) && next_X + 1 < 21)
            {
                inputManager.recent_user_input = KEY_RIGHT;
                tryMoveHeadTo(next_X + 1, next_Y, head_X, head_Y);
                return;
            }
            curr = root_map[next_Y + 1][next_X]; // KEY_DOWN
            if ((curr == 0 || curr == -15 || curr == -25) && next_Y + 1 < 21)
            {
                inputManager.recent_user_input = KEY_DOWN;
                tryMoveHeadTo(next_X, next_Y + 1, head_X, head_Y);
                return;
            }
            curr = root_map[next_Y - 1][next_X]; // KEY_UP
            if ((curr == 0 || curr == -15 || curr == -25) && next_Y - 1 >= 0)
            {
                inputManager.recent_user_input = KEY_UP;
                tryMoveHeadTo(next_X, next_Y - 1, head_X, head_Y);
                return;
            }
            curr = root_map[next_Y][next_X - 1]; // KEY_LEFT
            if ((curr == 0 || curr == -15 || curr == -25) && next_X - 1 >= 0)
            {
                inputManager.recent_user_input = KEY_LEFT;
                tryMoveHeadTo(next_X - 1, next_Y, head_X, head_Y);
                return;
            }
        }

        // <= KEY_LEFT
        else if (inputManager.recent_user_input == KEY_LEFT)
        {
            // 왼 -> 위 -> 아래 -> 위 순으로 진출
            int curr = root_map[next_Y][next_X - 1]; // KEY_LEFT
            if ((curr == 0 || curr == -15 || curr == -25) && next_X - 1 >= 0)
            {
                inputManager.recent_user_input = KEY_LEFT;
                tryMoveHeadTo(next_X - 1, next_Y, head_X, head_Y);
                return;
            }
            curr = root_map[next_Y - 1][next_X]; // KEY_UP
            if ((curr == 0 || curr == -15 || curr == -25) && next_Y - 1 >= 0)
            {
                inputManager.recent_user_input = KEY_UP;
                tryMoveHeadTo(next_X, next_Y - 1, head_X, head_Y);
                return;
            }
            curr = root_map[next_Y + 1][next_X]; // KEY_DOWN
            if ((curr == 0 || curr == -15 || curr == -25) && next_Y + 1 < 21)
            {
                inputManager.recent_user_input = KEY_DOWN;
                tryMoveHeadTo(next_X, next_Y + 1, head_X, head_Y);
                return;
            }
            curr = root_map[next_Y][next_X + 1]; // KEY_RIGHT
            if ((curr == 0 || curr == -15 || curr == -25) && next_X + 1 < 21)
            {
                inputManager.recent_user_input = KEY_RIGHT;
                tryMoveHeadTo(next_X + 1, next_Y, head_X, head_Y);
                return;
            }
        }
    }
}