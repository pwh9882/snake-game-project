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
    game_window = subwin(stdscr, 21, 22, 1, 1);
    init_pair(2, COLOR_BLACK, COLOR_WHITE); // 윈도우 백그라운드 컬러 box(win, 0, 0);
    attron(COLOR_PAIR(2));
    wbkgd(game_window, COLOR_PAIR(2));

    debug_window = subwin(stdscr, 5, 25, 1, 25);
    init_pair(3, COLOR_BLACK, COLOR_WHITE); // 윈도우 백그라운드 컬러 box(win, 0, 0);
    attron(COLOR_PAIR(3));
    wbkgd(debug_window, COLOR_PAIR(3));

    score_window = subwin(stdscr, 5, 25, 7, 25);
    init_pair(4, COLOR_BLACK, COLOR_WHITE); // 윈도우 백그라운드 컬러 box(win, 0, 0);
    attron(COLOR_PAIR(4));
    wbkgd(score_window, COLOR_PAIR(4));

    goal_window = subwin(stdscr, 5, 25, 13, 25);
    init_pair(5, COLOR_BLACK, COLOR_WHITE); // 윈도우 백그라운드 컬러 box(win, 0, 0);
    attron(COLOR_PAIR(5));
    wbkgd(goal_window, COLOR_PAIR(5));
}

void RenderManager::renderScreen()
{
    renderGameScreen();
    renderDebugScreen();
    renderScoreScreen();
    renderGoalScreen();
}

void RenderManager::renderGameScreen()
{  
    init_pair(10, COLOR_WHITE, COLOR_GREEN);
    init_pair(11, COLOR_WHITE, COLOR_RED);
    init_pair(12, COLOR_WHITE, COLOR_MAGENTA);
    init_pair(13, COLOR_WHITE, COLOR_YELLOW);

    for (int i = 0; i < 21; i++)
    {
        for (int j = 0; j < 21; j++)
        {          
            int curr = stageManager.root_map[i][j];
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
    wprintw(score_window, "Current Length: %d\n", stageManager.current_snack_length);
    wprintw(score_window, "Max Length: %d\n", stageManager.max_snack_length);
    wprintw(score_window, "Growth Earned: %d\n", stageManager.growth_item_count);
    wprintw(score_window, "Posion Earned: %d\n", stageManager.posion_item_count);
    wprintw(score_window, "Gate Passed: %d\n", stageManager.gate_passed_count);
}
void RenderManager::renderGoalScreen()
{
    wprintw(goal_window, "goal_window_preview");
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
