#include "mangers.h"
#include <ncurses.h>
extern InputManager inputManager;
extern GameManager gameManager;

/*
nothing: 0
snake_head:-1
snake_body:1~INT_MAX;
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

    game_window = subwin(stdscr, gameManager.map_height, gameManager.map_width + 1, 1, 1);
    init_pair(2, COLOR_BLACK, COLOR_WHITE); // 윈도우 백그라운드 컬러 box(win, 0, 0);
    attron(COLOR_PAIR(2));
    wbkgd(game_window, COLOR_PAIR(2));

    score_window = subwin(stdscr, 10, 25, 1, gameManager.map_width + 4);
    init_pair(4, COLOR_BLACK, COLOR_WHITE); // 윈도우 백그라운드 컬러 box(win, 0, 0);
    attron(COLOR_PAIR(4));
    wbkgd(score_window, COLOR_PAIR(4));

    goal_window = subwin(stdscr, 5, 25, 12, gameManager.map_width + 4);
    init_pair(5, COLOR_BLACK, COLOR_WHITE); // 윈도우 백그라운드 컬러 box(win, 0, 0);
    attron(COLOR_PAIR(5));
    wbkgd(goal_window, COLOR_PAIR(5));

    debug_window = subwin(stdscr, 5, 25, 18, gameManager.map_width + 4);
    init_pair(3, COLOR_BLACK, COLOR_WHITE); // 윈도우 백그라운드 컬러 box(win, 0, 0);
    attron(COLOR_PAIR(3));
    wbkgd(debug_window, COLOR_PAIR(3));
}

void RenderManager::renderScreen(int thick_count)
{
    clear();
    wclear(game_window);
    wclear(score_window);
    wclear(goal_window);
    wclear(debug_window);

    wprintw(stdscr, "%d\n", thick_count);
    renderGameScreen();
    renderScoreScreen();
    renderMissionScreen();
    renderDebugScreen();

    refresh();
    wrefresh(game_window);
    wrefresh(score_window);
    wrefresh(goal_window);
    wrefresh(debug_window);
}

void RenderManager::renderGameScreen()
{
    init_pair(10, COLOR_WHITE, COLOR_GREEN);
    init_pair(11, COLOR_WHITE, COLOR_RED);
    init_pair(12, COLOR_WHITE, COLOR_MAGENTA);
    init_pair(13, COLOR_WHITE, COLOR_YELLOW);
    init_pair(14, COLOR_WHITE, COLOR_CYAN);

    for (int i = 0; i < gameManager.map_height; i++)
    {
        for (int j = 0; j < gameManager.map_width; j++)
        {
            int curr = gameManager.current_game_map[i][j];
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
            else if (curr == -35)
            {
                wattron(game_window, A_BLINK | A_BOLD | COLOR_PAIR(14));
                waddch(game_window, 'R');
                wattroff(game_window, A_BLINK | A_BOLD | COLOR_PAIR(14));
            }
            else if (curr < -100)
            {
                waddch(game_window, -100 - curr + '0');
            }
        }
        wprintw(game_window, "\n");
    }
}

void RenderManager::renderDebugScreen()
{
    if (inputManager.recent_user_input != 0)
    {

        wprintw(debug_window, "The pressed key is ");
        attron(A_BOLD);
        wprintw(debug_window, "%c\n", inputManager.recent_user_input);
        wprintw(debug_window, "item cooltime: %d\n", gameManager.item_spawn_cooltime_counter);
        wprintw(debug_window, "game_over_flag: %d\n", gameManager.game_over_flag);
        wprintw(debug_window, "gate cooltime: %d\n", gameManager.gate_spawn_cooltime_counter);
        wprintw(debug_window, "passing flag: %d\n", gameManager.gate_passing_required_count);
        attroff(A_BOLD);
    }
}

void RenderManager::renderScoreScreen()
{
    wprintw(score_window, " Score Board\n");
    wprintw(score_window, " Elapsed Time: %d\n", gameManager.current_game_elapsed_time);
    wprintw(score_window, " Current Length: %d\n", gameManager.current_snake_length);
    wprintw(score_window, " Max Length: %d\n", gameManager.top_snake_length);
    wprintw(score_window, " Current Speed: %d\n", gameManager.current_game_speed);
    wprintw(score_window, " Growth Earned: %d\n", gameManager.growth_item_count);
    wprintw(score_window, " Posion Earned: %d\n", gameManager.posion_item_count);
    wprintw(score_window, " Gate Passed: %d\n", gameManager.gate_passed_count);
    wprintw(score_window, " Total Score: \n %d\n", gameManager.total_score);
}
void RenderManager::renderMissionScreen()
{
    attron(A_BOLD);
    wprintw(goal_window, " Mission \n");
    wprintw(
        goal_window, " B: (%d / %d), (%c)\n",
        gameManager.current_snake_length,
        gameManager.curr_stage->snake_length_goal,
        (gameManager.current_snake_length >= gameManager.curr_stage->snake_length_goal ? 'v' : ' '));

    wprintw(
        goal_window, " +: (%d / %d), (%c)\n",
        gameManager.growth_item_count,
        gameManager.curr_stage->growth_item_goal,
        (gameManager.growth_item_count >= gameManager.curr_stage->growth_item_goal ? 'v' : ' '));
    wprintw(
        goal_window, " -: (%d / %d), (%c)\n",
        gameManager.posion_item_count, gameManager.curr_stage->posion_item_goal,
        gameManager.posion_item_count >= gameManager.curr_stage->posion_item_goal ? 'v' : ' ');
    wprintw(
        goal_window, " G: (%d / %d), (%c)\n",
        gameManager.gate_passed_count, gameManager.curr_stage->gate_pass_goal,
        gameManager.gate_passed_count >= gameManager.curr_stage->gate_pass_goal ? 'v' : ' '

    );
    attroff(A_BOLD);
}
