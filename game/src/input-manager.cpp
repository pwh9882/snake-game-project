#include "mangers.h"
#include <ncurses.h>
InputManager::InputManager()
{
    recent_user_input = KEY_LEFT;
}
void InputManager::handleInput()
{
    int newVal = getch();
    if (newVal != ERR && (newVal == KEY_LEFT ||
                          newVal == KEY_RIGHT ||
                          newVal == KEY_UP ||
                          newVal == KEY_DOWN))
    {
        recent_user_input = newVal;
    }
}