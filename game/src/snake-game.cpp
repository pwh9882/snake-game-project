#include <iostream>
#include <chrono>
#include <ncurses.h>
#include "mangers.h"

using namespace std;
using namespace chrono;

const int FPS_SET = 60;                             // 기본 출력 fps 설정
using frame = duration<int32_t, ratio<1, FPS_SET>>; // FPS_SET fps를 위한 시간 단위
using ms = duration<float, milli>;                  // 밀리초 단위

GameManager gameManager;
InputManager inputManager;
RenderManager renderManager;
StageManager stageManager;

void startGame(int stage_index)
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
    gameManager.initGame(stage_index);
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

            if (thick_count % (FPS_SET / gameManager.current_game_speed) == 0)
            {
                // update game
                gameManager.updateGame();
                gameManager.calcTotalScore();

                if (
                    gameManager.current_snake_length >= gameManager.curr_stage->snack_length_goal &&
                    gameManager.growth_item_count >= gameManager.curr_stage->growth_item_goal &&
                    gameManager.posion_item_count >= gameManager.curr_stage->posion_item_goal &&
                    gameManager.gate_passed_count >= gameManager.curr_stage->gate_pass_goal)
                {
                    gameManager.game_complete_flag = true;
                }
                if (inputManager.recent_user_input == 'q' || gameManager.game_over_flag || gameManager.game_complete_flag)
                {
                    gameManager.endGame();
                    break;
                }

                // render screen
                renderManager.renderScreen(thick_count);
            }

            if (thick_count > FPS_SET)
            {
                gameManager.item_spawn_cooltime_counter++;
                gameManager.gate_spawn_cooltime_counter++;
                gameManager.current_game_elapsed_time++;
                thick_count = 1;
                if (gameManager.current_game_elapsed_time % 10 == 0)
                {
                    gameManager.current_game_speed++;
                }
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
    int stage_index = 0;
    // startGame(stage_index);
    while (true)
    {
        string user_input = "";
        if (gameManager.game_complete_flag == true && stage_index < 3)
        {

            cout << "클리어! 다음스테이지로 가고 싶으면 y를 입력 후 엔터, 종료하고 싶으면 q를 입력:\n";
            cin >> user_input;
            if (user_input == "y")
            {
                startGame(++stage_index);
            }
            else if (user_input == "q")
            {
                cout << "게임 종료됨.\n";
                break;
            }
        }
        else
        {
            cout << "게임을 시작하려면 1~4번(stage) 입력 후 엔터: 종료는 q\n";
            cin >> user_input;
            if (user_input == "1")
            {
                startGame(stage_index = 0);
            }
            else if (user_input == "2")
            {
                startGame(stage_index = 1);
            }
            else if (user_input == "3")
            {
                startGame(stage_index = 2);
            }
            else if (user_input == "4")
            {
                startGame(stage_index = 3);
            }
            else if (user_input == "q")
            {
                cout << "게임 종료됨.\n";
                break;
            }
        }
    }

    return 0;
}
