#include <ncurses.h>

class RenderManager
{
public:
    WINDOW *game_window;
    WINDOW *score_window;
    WINDOW *goal_window;
    WINDOW *debug_window;
    RenderManager();
    void initWindows(WINDOW *stdscr);
    void renderScreen();
    void renderGameScreen();
    void renderScoreScreen();
    void renderGoalScreen();
    void renderDebugScreen();
};