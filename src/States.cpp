#include "States.h"
#include "raylib.h"
#include "Settings.hpp"

States::States()
{
    Load();
}

void States::Load()
{
    if (Settings::Get().windowHeight == 0 || Settings::Get().windowWidth == 0) {
        width = GetScreenWidth();
        height = GetScreenHeight();
    }
    else {
        width = Settings::Get().windowWidth;
        height = Settings::Get().windowHeight;
    }
    title = "Island Project"; // Title name
}
