#include "LiteDebugger.h"

#include <raylib.h>

static void UpdateDebugCursor(DebugSettings& debugSettings, const bool wasExpanded)
{
    if (!wasExpanded && debugSettings.isExpanded)
    {
        debugSettings.isCursorPreviouslyEnabled = !IsCursorHidden();
        EnableCursor();
    }
    else if (wasExpanded && !debugSettings.isExpanded)
    {
        if (debugSettings.isCursorPreviouslyEnabled)
        {
            EnableCursor();
        }
        else
        {
            DisableCursor();
        }
    }
}

void CheckDebugKeys(DebugSettings& debugSettings)
{
    const bool wasExpanded = debugSettings.isExpanded;
    if (IsKeyPressed(KEY_F3))
    {
        const bool isShiftHeld = IsKeyDown(KEY_LEFT_SHIFT) || IsKeyDown(KEY_RIGHT_SHIFT);
        if (isShiftHeld)
        {
            debugSettings.inDebugMode = true;
            debugSettings.isExpanded = !debugSettings.isExpanded;
        }
        else
        {
            debugSettings.inDebugMode = !debugSettings.inDebugMode;
            debugSettings.isExpanded = false;
        }
    }

    if (IsKeyPressed(KEY_F4))
    {
        if (!debugSettings.isExpanded)
        {
            debugSettings.inDebugMode = true;
            debugSettings.isExpanded = true;
            debugSettings.isConsoleVisible = true;
        }
        else
        {
            debugSettings.isConsoleVisible = !debugSettings.isConsoleVisible;
        }
    }
    UpdateDebugCursor(debugSettings, wasExpanded);

    if (!debugSettings.inDebugMode)
    {
        return;
    }
    if (IsKeyPressed(KEY_H))
    {
        debugSettings.Show3DGrid = !debugSettings.Show3DGrid;
    }
    if (IsKeyPressed(KEY_F))
    {
        debugSettings.ShowFps = !debugSettings.ShowFps;
    }
    if (IsKeyPressed(KEY_P))
    {
        debugSettings.Show3DPosition = !debugSettings.Show3DPosition;
    }
    if (IsKeyPressed(KEY_C))
    {
        debugSettings.Show2DPosition = !debugSettings.Show2DPosition;
    }
}

void UpdateDebugInput()
{
    DebugSettings& debugSettings = DebugSettings::Get();
    CheckDebugKeys(debugSettings);

    static const double statisticsStartTime = GetTime();
    static double lastMemorySampleTime = -1;
    const double currentTime = GetTime();

    // Ignore startup/resource-loading frames; memory polling stays at once per second.
    if (currentTime - statisticsStartTime >= 1.0)
    {
        debugSettings.frameStatistics.AddFrame(GetFrameTime());
    }
    if (currentTime - lastMemorySampleTime >= 1.0)
    {
        debugSettings.memoryStatistics.Sample();
        lastMemorySampleTime = currentTime;
    }
}

void UpdateDebug()
{
    DebugSettings& debugSettings = DebugSettings::Get();
    if (debugSettings.inDebugMode)
    {
        UpdateDebugGUI(debugSettings);
    }
}
