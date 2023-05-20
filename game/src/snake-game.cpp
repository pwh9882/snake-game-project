#include <iostream>
#include <chrono>
#include <ncurses.h>
#include "mangers.h"

using namespace std;
using namespace chrono;

const int FPS_SET = 60;                             // 기본 출력 fps 설정
using frame = duration<int32_t, ratio<1, FPS_SET>>; // FPS_SET fps를 위한 시간 단위
using ms = duration<float, milli>;                  // 밀리초 단위

StageManager stageManager;
InputManager inputManager;
RenderManager renderManager;

void startGame()
{
    initscr();
    start_color();
    // init_pair(1, COLOR_RED, COLOR_GREEN);
    // attron(COLOR_PAIR(1));
    // wbkgd(stdscr, COLOR_PAIR(1));
    box(stdscr, 0, 0);

    raw();                /* Line buffering disabled      */
    keypad(stdscr, TRUE); /* We get F1, F2 etc..          */
    noecho();             /* Don't echo() while we do getch */
    timeout(0.0001);

    time_point<steady_clock> fpsTimer(steady_clock::now()); // 타이머 시작
    frame FPS{};                                            // 프레임 카운터
    int thick_count = 0;
    stageManager.initStage();
    renderManager.initWindows(stdscr);
    inputManager.recent_user_input = KEY_LEFT;
    while (true)
    {
        // handleInput
        inputManager.handleInput();

        FPS = duration_cast<frame>(steady_clock::now() - fpsTimer); // 현재 프레임과 타이머의 차이를 계산
        if (FPS.count() >= 1)                                       // 1프레임 이상 지났으면
        {
            fpsTimer = steady_clock::now(); // 타이머 재설정
                                            // cout << "LastFrame: " << duration_cast<ms>(FPS).count() << "ms | FPS: " << FPS.count() * FPS_SET << endl; // 프레임 정보 출력

            thick_count++;

            if (thick_count % (FPS_SET / stageManager.current_game_speed) == 0)
            {
                if (inputManager.recent_user_input == 'q' || stageManager.game_over_flag)
                {
                    break;
                }
                // update game
                stageManager.updateGame();

                // render screen
                clear();
                wclear(renderManager.game_window);
                wclear(renderManager.debug_window);
                wclear(renderManager.score_window);
                wclear(renderManager.goal_window);

                wprintw(stdscr, "%d\n", thick_count);
                renderManager.renderScreen();

                refresh();
                wrefresh(renderManager.game_window);
                wrefresh(renderManager.debug_window);
                wrefresh(renderManager.score_window);
                wrefresh(renderManager.goal_window);
            }

            if (thick_count > FPS_SET)
            {
                stageManager.item_spawn_cooltime_counter++;
                stageManager.gate_spawn_cooltime_counter++;
                stageManager.current_game_elapsed_time++;
                thick_count = 1;
            }
        }
    }

    delwin(renderManager.game_window);
    delwin(renderManager.debug_window);
    delwin(renderManager.score_window);
    delwin(renderManager.goal_window);
    endwin();
}

int main()
{

    while (true)
    {
        int user_input = -1;
        cout << "게임을 시작하려면 1번 입력: 종료는 2번";
        cin >> user_input;
        if (user_input == 1)
        {
            startGame();
        }
        else if (user_input == 2)
        {
            cout << "게임 종료됨.";
            break;
        }
    }

    return 0;
}
