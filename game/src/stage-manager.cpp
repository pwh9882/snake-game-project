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
    game_progress_counter = 0;
}
void StageManager::updateGame()
{
    int headX = 0, headY = 0;
    // 머리 찾기
    for (int i = 0; i < 21; i++)
    {
        for (int j = 0; j < 21; j++)
        {
            int curr = root_map[i][j];
            if (curr == -1)
            {
                headX = i;
                headY = j;
            }
        }
        // printw("\n");
    }

    // ======================================
    // 머리이동
    int nextX = headX, nextY = headY;
    switch (inputManager.recent_user_input)
    {
    case KEY_LEFT:
        nextY--;
        break;
    case KEY_RIGHT:
        nextY++;
        break;
    case KEY_UP:
        nextX--;
        break;
    case KEY_DOWN:
        nextX++;
        break;
    }

    // ======================================
    // 아무것도 없으면 이동
    if (root_map[nextX][nextY] == 0)
    {
        root_map[headX][headY] = current_snack_length;
        root_map[nextX][nextY] = -1;
    }
    // 성장아이템 먹으면 길이 + 1
    else if (root_map[nextX][nextY] >= -15 && root_map[nextX][nextY] < -10)
    {
        root_map[headX][headY] = current_snack_length++;
        root_map[nextX][nextY] = -1;
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
    else if (root_map[nextX][nextY] >= -25 && root_map[nextX][nextY] < -20)
    {
        root_map[headX][headY] = current_snack_length--;
        root_map[nextX][nextY] = -1;
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
            // else if (curr >= -15 && curr < -10)
            // {
            //     curr++;
            // }
            // else if (curr >= -25 && curr < -20)
            // {
            //     curr++;
            // }

            // if (curr == -10 || curr == -20)
            // {
            //     curr = 0;
            // }
        }
    }

    // 틱 마다 아이템 생성
    if (game_progress_counter == 5)
    {
        game_progress_counter = 0;
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

void StageManager::getRandomCordForItem()
{
    int a = 0;
}