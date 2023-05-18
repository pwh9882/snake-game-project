#include <iostream>
#include <chrono>
#include <cstdint>
#include <ncurses.h>
#include "mangers.h"

using namespace std;
using namespace chrono;

const int FPS_SET = 60;                             // 기본 출력 fps 설정
const int RENDERFPS = 2;                            // 1초에 얼마나 렌더링 할 것인지
using frame = duration<int32_t, ratio<1, FPS_SET>>; // FPS_SET fps를 위한 시간 단위
using ms = duration<float, milli>;                  // 밀리초 단위

StageManager stageManager;
InputManager inputManager;
RenderManager renderManager;

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
    stageManager.initStage();
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
            if (thick_count % (FPS_SET / RENDERFPS) == 0)
            {

                // update game
                stageManager.updateGame();

                // render screen
                clear();
                printw("%d\n", thick_count);
                renderManager.renderScreen();
            }

            if (thick_count > FPS_SET)
            {
                stageManager.game_progress_counter++;
                thick_count = 1;
            }
        }
    }
    return 0;
}