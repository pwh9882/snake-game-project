#include "input-manager.h"
#include "stage-manager.h"
#include "render-manager.h"
#include "game-manager.h"

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
enum CODE
{
    HEAD = -1,
    NOTING = 0,
    IMMUNE_WALL = -2,
    WALL = -3,
    GATE = -4,
    GROWTH = -15,
    POSION = -25,
    REVERSE = -35,
};