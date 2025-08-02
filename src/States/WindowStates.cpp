#include "States.h"
#include "raylib.h"
#include "Settings.hpp"

WindowStates::WindowStates()
{
    Load();
}

void WindowStates::Load()
{
    if (Settings::Get().windowHeight == 0 || Settings::Get().windowWidth == 0) {
        currentMonitor = 0;
        width = GetMonitorWidth(currentMonitor);
        height = GetMonitorHeight(currentMonitor);
    }
    else {
        width = Settings::Get().windowWidth;
        height = Settings::Get().windowHeight;
        currentMonitor = Settings::Get().currentMonitor;
    }
    title = "Island Project"; // Title name
}
