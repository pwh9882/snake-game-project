#ifndef _RENDER_MANAGER_H_
#define _RENDER_MANAGER_H_

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
    void renderScreen(int thick_count);
    void renderGameScreen();
    void renderScoreScreen();
    void renderMissionScreen();
    void renderDebugScreen();
};

#endif