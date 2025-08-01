#include "States.h"

GameState::GameState()
{
    screenState = MAIN_MENU;
    isScreenOnTransition = false;
    isFinished = false;
}
