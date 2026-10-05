#include "DebugDrawing.h"
#include "LiteDebugger.h"

#include <algorithm>

static constexpr int consoleLineHeight = 19;
static int consoleScrollOffset = 0;

static void DrawDebugConsoleControls(const Rectangle bounds, DebugSettings& debugSettings)
{
    DrawDebugPanel(bounds, "CONSOLE  /  cout + cerr");

    const Rectangle clearButtonBounds = {bounds.x + bounds.width - 76, bounds.y + 7, 62, 25};
    if (DrawDebugButton(clearButtonBounds, "Clear"))
    {
        ClearDebugLog();
        consoleScrollOffset = 0;
    }

    const Rectangle followButtonBounds = {bounds.x + 14, bounds.y + 46, 116, 25};
    const char* followButtonLabel = debugSettings.isConsoleFollowing ? "Follow: on" : "Follow: off";
    if (DrawDebugButton(followButtonBounds, followButtonLabel))
    {
        debugSettings.isConsoleFollowing = !debugSettings.isConsoleFollowing;
    }
}

static void UpdateDebugConsoleScroll(const Rectangle contentBounds, const int maximumScroll,
                                    DebugSettings& debugSettings)
{
    if (CheckCollisionPointRec(GetMousePosition(), contentBounds))
    {
        const float mouseWheelMovement = GetMouseWheelMove();
        if (mouseWheelMovement != 0)
        {
            if (debugSettings.isConsoleFollowing)
            {
                consoleScrollOffset = maximumScroll;
            }
            debugSettings.isConsoleFollowing = false;
            consoleScrollOffset -= static_cast<int>(mouseWheelMovement * 3);
        }

        if (IsKeyPressed(KEY_HOME))
        {
            debugSettings.isConsoleFollowing = false;
            consoleScrollOffset = 0;
        }
        if (IsKeyPressed(KEY_END))
        {
            debugSettings.isConsoleFollowing = true;
        }
    }

    if (debugSettings.isConsoleFollowing)
    {
        consoleScrollOffset = maximumScroll;
    }
    else
    {
        consoleScrollOffset = std::clamp(consoleScrollOffset, 0, maximumScroll);
    }
}

static void DrawDebugConsoleLines(const Rectangle contentBounds, const int visibleLineCount,
                                 const std::vector<std::string>& logLines)
{
    BeginDebugClip(contentBounds);
    if (logLines.empty())
    {
        DrawDebugText("Waiting for std::cout / std::cerr...", contentBounds.x, contentBounds.y, debugMutedColor);
    }

    const int logLineCount = static_cast<int>(logLines.size());
    for (int visibleRow = 0; visibleRow < visibleLineCount; ++visibleRow)
    {
        const int logLineIndex = consoleScrollOffset + visibleRow;
        if (logLineIndex >= logLineCount)
        {
            break;
        }

        const float linePositionY = contentBounds.y + static_cast<float>(visibleRow * consoleLineHeight);
        DrawDebugText(logLines[logLineIndex].c_str(), contentBounds.x, linePositionY);
    }
    EndScissorMode();
}

static void DrawDebugConsoleScrollBar(const Rectangle panelBounds, const Rectangle contentBounds,
                                     const int visibleLineCount, const int logLineCount, const int maximumScroll)
{
    if (maximumScroll <= 0)
    {
        return;
    }

    const float visibleLinesHeight = contentBounds.height * static_cast<float>(visibleLineCount);
    const float thumbHeight = std::max(12.0f, visibleLinesHeight / static_cast<float>(logLineCount));
    const float scrollDistance = (contentBounds.height - thumbHeight) * static_cast<float>(consoleScrollOffset);
    const float thumbPositionY = contentBounds.y + scrollDistance / static_cast<float>(maximumScroll);
    const Rectangle thumbBounds = {panelBounds.x + panelBounds.width - 6, thumbPositionY, 3, thumbHeight};
    DrawRectangleRec(thumbBounds, debugAccentColor);
}

void DrawDebugConsole(const Rectangle bounds, DebugSettings& debugSettings)
{
    DrawDebugConsoleControls(bounds, debugSettings);

    const std::vector<std::string> logLines = GetDebugLogLines();
    const int logLineCount = static_cast<int>(logLines.size());
    const int maximumLogLineCount = static_cast<int>(maximumDebugLogLines);
    DrawDebugText(TextFormat("%i / %i lines", logLineCount, maximumLogLineCount), bounds.x + 144, bounds.y + 51, debugMutedColor);

    const Rectangle contentBounds = {bounds.x + 12, bounds.y + 82, bounds.width - 24,
                                     std::max(0.0f, bounds.height - 98)};
    const int visibleLineCount = std::max(1, static_cast<int>(contentBounds.height / consoleLineHeight));
    const int maximumScroll = std::max(0, logLineCount - visibleLineCount);

    UpdateDebugConsoleScroll(contentBounds, maximumScroll, debugSettings);
    DrawDebugConsoleLines(contentBounds, visibleLineCount, logLines);
    DrawDebugConsoleScrollBar(bounds, contentBounds, visibleLineCount, logLineCount, maximumScroll);
}
