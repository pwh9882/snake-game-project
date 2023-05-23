#include "mangers.h"
#include <ncurses.h>
extern InputManager inputManager;
extern StageManager stageManager;

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
RenderManager::RenderManager()
{
}

void RenderManager::initWindows(WINDOW *stdscr)
{

    game_window = subwin(stdscr, stageManager.map_height, stageManager.map_width + 1, 1, 1);
    init_pair(2, COLOR_BLACK, COLOR_WHITE); // 윈도우 백그라운드 컬러 box(win, 0, 0);
    attron(COLOR_PAIR(2));
    wbkgd(game_window, COLOR_PAIR(2));

    debug_window = subwin(stdscr, 5, 25, 1, stageManager.map_width + 4);
    init_pair(3, COLOR_BLACK, COLOR_WHITE); // 윈도우 백그라운드 컬러 box(win, 0, 0);
    attron(COLOR_PAIR(3));
    wbkgd(debug_window, COLOR_PAIR(3));

    score_window = subwin(stdscr, 6, 25, 7, stageManager.map_width + 4);
    init_pair(4, COLOR_BLACK, COLOR_WHITE); // 윈도우 백그라운드 컬러 box(win, 0, 0);
    attron(COLOR_PAIR(4));
    wbkgd(score_window, COLOR_PAIR(4));

    goal_window = subwin(stdscr, 5, 25, 14, stageManager.map_width + 4);
    init_pair(5, COLOR_BLACK, COLOR_WHITE); // 윈도우 백그라운드 컬러 box(win, 0, 0);
    attron(COLOR_PAIR(5));
    wbkgd(goal_window, COLOR_PAIR(5));
}

void RenderManager::renderScreen()
{
    renderGameScreen();
    renderDebugScreen();
    renderScoreScreen();
    renderMissionScreen();
}

void RenderManager::renderGameScreen()
{
    init_pair(10, COLOR_WHITE, COLOR_GREEN);
    init_pair(11, COLOR_WHITE, COLOR_RED);
    init_pair(12, COLOR_WHITE, COLOR_MAGENTA);
    init_pair(13, COLOR_WHITE, COLOR_YELLOW);

    for (int i = 0; i < stageManager.map_height; i++)
    {
        for (int j = 0; j < stageManager.map_width; j++)
        {
            int curr = stageManager.stage_map[i][j];
            if (curr > 0)
            {
                wattron(game_window, A_BLINK | A_BOLD | COLOR_PAIR(13));
                waddch(game_window, ' ');
                wattroff(game_window, A_BLINK | A_BOLD | COLOR_PAIR(13));
            }
            else if (curr == 0)
            {
                wprintw(game_window, " ");
            }
            else if (curr == -1)
            {
                wattron(game_window, A_BOLD | COLOR_PAIR(13));
                waddch(game_window, 'H');
                wattroff(game_window, A_BOLD | COLOR_PAIR(13));
            }
            else if (curr == -2)
            {
                waddch(game_window, char(219));
            }
            else if (curr == -3)
            {
                waddch(game_window, ACS_CKBOARD);
            }
            else if (curr == -4)
            {
                wattron(game_window, A_BOLD | COLOR_PAIR(12));
                waddch(game_window, 'G');
                wattroff(game_window, A_BOLD | COLOR_PAIR(12));
            }
            else if (curr == -15)
            {
                wattron(game_window, A_BLINK | A_BOLD | COLOR_PAIR(10));
                waddch(game_window, 'A');
                wattroff(game_window, A_BLINK | A_BOLD | COLOR_PAIR(10));
            }
            else if (curr == -25)
            {
                wattron(game_window, A_BLINK | A_BOLD | COLOR_PAIR(11));
                waddch(game_window, 'P');
                wattroff(game_window, A_BLINK | A_BOLD | COLOR_PAIR(11));
            }
        }
        wprintw(game_window, "\n");
    }
}

void RenderManager::renderScoreScreen()
{
    // wprintw(score_window, "score_window_preview\n");
    wprintw(score_window, "Elapsed Time: %d\n", stageManager.current_game_elapsed_time);
    wprintw(score_window, "Current Length: %d\n", stageManager.current_snake_length);
    wprintw(score_window, "Max Length: %d\n", stageManager.max_snake_length);
    wprintw(score_window, "Growth Earned: %d\n", stageManager.growth_item_count);
    wprintw(score_window, "Posion Earned: %d\n", stageManager.posion_item_count);
    wprintw(score_window, "Gate Passed: %d\n", stageManager.gate_passed_count);
}
void RenderManager::renderMissionScreen()
{
    attron(A_BOLD);
    wprintw(goal_window, "Mission \n");
    wprintw(goal_window, "Earn Growth: (%d / %d)\n", stageManager.growth_item_count, stageManager.growth_item_goal);
    wprintw(goal_window, "Earn Posion: (%d / %d)\n", stageManager.posion_item_count, stageManager.posion_item_goal);
    wprintw(goal_window, "pass Gate: (%d / %d)\n", stageManager.gate_passed_count, stageManager.gate_pass_goal);
    attroff(A_BOLD);
}

void RenderManager::renderDebugScreen()
{
    if (inputManager.recent_user_input != 0)
    {

        wprintw(debug_window, "The pressed key is ");
        attron(A_BOLD);
        wprintw(debug_window, "%c\n", inputManager.recent_user_input);
        wprintw(debug_window, "item cooltime: %d\n", stageManager.item_spawn_cooltime_counter);
        wprintw(debug_window, "game_over_flag: %d\n", stageManager.game_over_flag);
        wprintw(debug_window, "gate cooltime: %d\n", stageManager.gate_spawn_cooltime_counter);
        wprintw(debug_window, "passing flag: %d\n", stageManager.is_snake_passing_gate_flag);
        attroff(A_BOLD);
    }
}
