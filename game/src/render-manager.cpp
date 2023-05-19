#include "mangers.h"
#include <ncurses.h>
extern InputManager inputManager;
extern StageManager stageManager;
void RenderManager::renderScreen()
{
    if (inputManager.recent_user_input != 0)
    {
        printw("The pressed key is ");
        attron(A_BOLD);
        printw("%c\n", inputManager.recent_user_input);
        printw("%d\n", stageManager.game_progress_counter);
        attroff(A_BOLD);
    }

    for (int i = 0; i < 21; i++)
    {
        for (int j = 0; j < 21; j++)
        {
            int curr = stageManager.root_map[i][j];
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
            else if (curr == -4)
            {
                printw("O");
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