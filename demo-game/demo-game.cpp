#include <iostream>
#include <chrono>
#include <cstdint>
#include <ncurses.h>
using namespace std;
using namespace chrono;

const int FPS_SET = 60;                             // 기본 출력 fps 설정
const int RENDERFPS = 2;                            // 1초에 얼마나 렌더링 할 것인지
using frame = duration<int32_t, ratio<1, FPS_SET>>; // FPS_SET fps를 위한 시간 단위
using ms = duration<float, milli>;                  // 밀리초 단위

void handleInput(int &recent_user_input);
void updateGame(const int &recent_user_input);
void renderScreen(const int &recent_user_input);

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

int root_map[21][21] = {
    {-2, -3, -3, -3, -3, -3, -3, -3, -3, -3, -3, -3, -3, -3, -3, -3, -3, -3, -3, -3, -2},
    {-3, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, -3},
    {-3, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, -3},
    {-3, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, -3},
    {-3, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, -25, 00, 00, 00, 00, 00, 00, 00, 00, -3},
    {-3, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, -3},
    {-3, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, -3},
    {-3, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, -3},
    {-3, 00, 00, 00, 00, -15, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, -3},
    {-3, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, -3},
    {-3, 00, 00, 00, 00, 00, 00, 00, 00, -1, 03, 02, 01, 00, 00, 00, 00, 00, 00, 00, -3},
    {-3, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, -3},
    {-3, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, -3},
    {-3, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, -15, 00, 00, 00, 00, 00, 00, -3},
    {-3, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, -3},
    {-3, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, -3},
    {-3, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, -3},
    {-3, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, -3},
    {-3, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, -3},
    {-3, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, -3},
    {-2, -3, -3, -3, -3, -3, -3, -3, -3, -3, -3, -3, -3, -3, -3, -3, -3, -3, -3, -3, -2},
};
int start_length = 4;

int main()
{
    initscr();
    raw();                /* Line buffering disabled      */
    keypad(stdscr, TRUE); /* We get F1, F2 etc..          */
    noecho();             /* Don't echo() while we do getch */
    timeout(0.0001);

    time_point<steady_clock> fpsTimer(steady_clock::now()); // 타이머 시작
    frame FPS{};                                            // 프레임 카운터
    int thick_count = 0;
    int recent_user_input = KEY_LEFT;
    while (true)
    {
        // handleInput
        handleInput(recent_user_input);

        FPS = duration_cast<frame>(steady_clock::now() - fpsTimer); // 현재 프레임과 타이머의 차이를 계산
        if (FPS.count() >= 1)                                       // 1프레임 이상 지났으면
        {
            fpsTimer = steady_clock::now(); // 타이머 재설정
                                            // cout << "LastFrame: " << duration_cast<ms>(FPS).count() << "ms | FPS: " << FPS.count() * FPS_SET << endl; // 프레임 정보 출력

            thick_count++;
            if (thick_count % (FPS_SET / RENDERFPS) == 0)
            {

                // update game
                updateGame(recent_user_input);

                // render screen
                clear();
                printw("%d\n", thick_count);
                renderScreen(recent_user_input);
            }
            if (thick_count > FPS_SET)
                thick_count = 1;
        }
    }
    return 0;
}

void handleInput(int &recent_user_input)
{
    int newVal = getch();
    if (newVal != ERR && (newVal == KEY_LEFT ||
                          newVal == KEY_RIGHT ||
                          newVal == KEY_UP ||
                          newVal == KEY_DOWN))
    {
        recent_user_input = newVal;
    }
}

void updateGame(const int &recent_user_input)
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
    switch (recent_user_input)
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
        root_map[headX][headY] = start_length;
        root_map[nextX][nextY] = -1;
    }
    // 성장아이템 먹으면 길이 + 1
    if (root_map[nextX][nextY] == -15)
    {
        root_map[headX][headY] = start_length++;
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
        root_map[headX][headY] = start_length--;
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

void renderScreen(const int &recent_user_input)
{
    if (recent_user_input != 0)
    {
        printw("The pressed key is ");
        attron(A_BOLD);
        printw("%c\n", recent_user_input);
        attroff(A_BOLD);
    }

    for (int i = 0; i < 21; i++)
    {
        for (int j = 0; j < 21; j++)
        {
            int curr = root_map[i][j];
            if (curr > 0)
            {
                printw("@");
            }
            else if (curr == 0)
            {
                printw(".");
            }
            else if (curr == -1)
            {
                printw("&");
            }
            else if (curr == -2)
            {
                printw("#");
            }
            else if (curr == -3)
            {
                printw("=");
            }
            else if (curr == -15)
            {
                printw("*");
            }
            else if (curr == -25)
            {
                printw("X");
            }
        }
        printw("\n");
    }
}