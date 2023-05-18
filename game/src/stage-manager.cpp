#include "mangers.h"
#include <ncurses.h>
extern InputManager inputManager;
StageManager::StageManager()
{
    initStage();
}
void StageManager::initStage()
{
    current_snack_length = start_snack_length;
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
    // 아무것도 없으면 이동
    if (root_map[nextX][nextY] == 0)
    {
        root_map[headX][headY] = current_snack_length;
        root_map[nextX][nextY] = -1;
    }
    // 성장아이템 먹으면 길이 + 1
    if (root_map[nextX][nextY] == -15)
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
    if (root_map[nextX][nextY] == -25)
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

    // 몸통죽이기
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
