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
    wprintw(goal_window, "goal_window_preview");
    wprintw(score_window, "score_window_preview");
    if (inputManager.recent_user_input != 0)
    {

        wprintw(debug_window, "The pressed key is ");
        attron(A_BOLD);
        wprintw(debug_window, "%c\n", inputManager.recent_user_input);
        wprintw(debug_window, "%d\n", stageManager.game_progress_counter);
        attroff(A_BOLD);
    }

    for (int i = 0; i < 21; i++)
    {
        for (int j = 0; j < 21; j++)
        {
            int curr = stageManager.root_map[i][j];
            if (curr > 0)
            {
                wprintw(game_window, "@");
            }
            else if (curr == 0)
            {
                wprintw(game_window, ".");
            }
            else if (curr == -1)
            {
                wprintw(game_window, "&");
            }
            else if (curr == -2)
            {
                wprintw(game_window, "#");
            }
            else if (curr == -3)
            {
                wprintw(game_window, "=");
            }
            else if (curr == -4)
            {
                wprintw(game_window, "O");
            }
            else if (curr == -15)
            {
                wprintw(game_window, "*");
            }
            else if (curr == -25)
            {
                wprintw(game_window, "X");
            }
        }
        wprintw(game_window, "\n");
    }
}