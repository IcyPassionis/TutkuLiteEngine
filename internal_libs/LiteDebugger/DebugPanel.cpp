#include "DebugDrawing.h"
#include "LiteDebugger.h"

#include <algorithm>

static float detailsScrollOffset = 0;

static Rectangle GetDebugPanelBounds(const DebugSettings& debugSettings)
{
    const float screenWidth = static_cast<float>(GetScreenWidth());
    const float screenHeight = static_cast<float>(GetScreenHeight());
    float panelWidth = 360;
    float panelHeight = 218;

    if (debugSettings.isExpanded)
    {
        panelWidth = 430;
        panelHeight = 530;
        const bool isConsoleBelowPanel = screenWidth < 800 && debugSettings.isConsoleVisible;
        if (isConsoleBelowPanel)
        {
            panelHeight = std::min(panelHeight, screenHeight * 0.48f);
        }
        else
        {
            panelHeight = std::min(panelHeight, screenHeight - 32);
        }
    }
    else
    {
        panelHeight = std::min(panelHeight, screenHeight - 32);
    }

    panelWidth = std::min(panelWidth, screenWidth - 32);
    return {16, 16, panelWidth, panelHeight};
}

static Rectangle GetDebugConsoleBounds(const Rectangle panelBounds)
{
    const float screenWidth = static_cast<float>(GetScreenWidth());
    const float screenHeight = static_cast<float>(GetScreenHeight());
    if (screenWidth >= 800)
    {
        const float consoleWidth = std::min(840.0f, screenWidth - panelBounds.width - 48);
        const float consoleHeight = std::min(530.0f, screenHeight - 32);
        return {panelBounds.x + panelBounds.width + 16, 16, consoleWidth, consoleHeight};
    }

    const float consolePositionY = panelBounds.y + panelBounds.height + 12;
    const float consoleHeight = screenHeight - panelBounds.height - 44;
    return {16, consolePositionY, screenWidth - 32, consoleHeight};
}

static float GetDebugContentHeight(const DebugSettings& debugSettings, const int toggleColumnCount)
{
    if (!debugSettings.isExpanded)
    {
        return 140;
    }

    float contentHeight = 241;
    if (debugSettings.ShowFps)
    {
        contentHeight += 48;
    }
    if (debugSettings.ShowLatency)
    {
        contentHeight += 84;
    }
    if (debugSettings.Show3DPosition)
    {
        contentHeight += 24;
    }
    if (debugSettings.Show2DPosition)
    {
        contentHeight += 24;
    }

    const int toggleRowCount = 6 / toggleColumnCount;
    contentHeight += static_cast<float>(toggleRowCount * 27);
    return contentHeight;
}

static void UpdateDebugPanelScroll(const Rectangle contentBounds, const float contentHeight, const bool isExpanded)
{
    const float maximumScroll = std::max(0.0f, contentHeight - contentBounds.height);
    if (isExpanded && CheckCollisionPointRec(GetMousePosition(), contentBounds))
    {
        detailsScrollOffset -= GetMouseWheelMove() * 32;
    }
    detailsScrollOffset = std::clamp(detailsScrollOffset, 0.0f, maximumScroll);
}

static std::string FormatDebugMemory(const bool isAvailable, const std::uint64_t bytes, const std::uint64_t peakBytes)
{
    if (!isAvailable)
    {
        return "N/A";
    }

    const double currentMebibytes = static_cast<double>(bytes) / 1048576.0;
    const double peakMebibytes = static_cast<double>(peakBytes) / 1048576.0;
    return TextFormat("%.1f / %.1f MiB", currentMebibytes, peakMebibytes);
}

static void DrawDebugPerformanceRows(const Rectangle panelBounds, float& nextRowY, const DebugSettings& debugSettings)
{
    const DebugFrameStatistics& statistics = debugSettings.frameStatistics;
    if (debugSettings.ShowFps)
    {
        DrawDebugRow(panelBounds, nextRowY, "FPS / average",
                     TextFormat("%.0f / %.0f", statistics.currentFps, statistics.averageFps));
        DrawDebugRow(panelBounds, nextRowY, "Min / max FPS",
                     TextFormat("%.0f / %.0f", statistics.minimumFps, statistics.maximumFps));
    }
    DrawDebugRow(panelBounds, nextRowY, "Frame time", TextFormat("%.2f ms", statistics.frameMilliseconds));

    const DebugMemoryStatistics& memory = debugSettings.memoryStatistics;
    const std::string ramText = FormatDebugMemory(memory.isRamAvailable, memory.ramBytes, memory.peakRamBytes);
    const std::string vramText = FormatDebugMemory(memory.isVramAvailable, memory.vramBytes, memory.peakVramBytes);
    DrawDebugRow(panelBounds, nextRowY, "RAM / peak", ramText);
    DrawDebugRow(panelBounds, nextRowY, "VRAM / peak", vramText);
}

static Vector2 GetDebugGraphPosition(const Rectangle bounds, const std::size_t sampleIndex,
                                    const std::size_t sampleCapacity, const float milliseconds,
                                    const float maximumMilliseconds)
{
    const float positionX = bounds.x + static_cast<float>(sampleIndex) * bounds.width / static_cast<float>(sampleCapacity - 1);
    const float positionY = bounds.y + bounds.height - milliseconds / maximumMilliseconds * bounds.height;
    return {positionX, positionY};
}

static void DrawDebugFrameGraph(const Rectangle bounds, const DebugFrameStatistics& statistics)
{
    DrawRectangleRec(bounds, {11, 16, 24, 230});
    float maximumMilliseconds = 33.3f;
    for (const float milliseconds : statistics.frameTimes)
    {
        maximumMilliseconds = std::max(maximumMilliseconds, milliseconds);
    }

    const float referencePositionY = bounds.y + bounds.height - (16.67f / maximumMilliseconds) * bounds.height;
    DrawLineEx({bounds.x, referencePositionY}, {bounds.x + bounds.width, referencePositionY}, 1, debugBorderColor);
    DrawDebugText("16.7 ms", bounds.x + 6, referencePositionY - 16, debugMutedColor);

    const std::size_t sampleCapacity = statistics.frameTimes.size();
    const std::size_t oldestSampleIndex = statistics.nextFrameIndex + sampleCapacity - statistics.frameCount;
    for (std::size_t sampleIndex = 1; sampleIndex < statistics.frameCount; ++sampleIndex)
    {
        const std::size_t previousSampleIndex = (oldestSampleIndex + sampleIndex - 1) % sampleCapacity;
        const std::size_t currentSampleIndex = (previousSampleIndex + 1) % sampleCapacity;
        const Vector2 previousPosition = GetDebugGraphPosition(bounds, sampleIndex - 1, sampleCapacity,
                                                               statistics.frameTimes[previousSampleIndex], maximumMilliseconds);
        const Vector2 currentPosition = GetDebugGraphPosition(bounds, sampleIndex, sampleCapacity,
                                                              statistics.frameTimes[currentSampleIndex], maximumMilliseconds);
        DrawLineEx(previousPosition, currentPosition, 1, debugAccentColor);
    }
}

static void DrawDebugSceneRows(const Rectangle panelBounds, float& nextRowY, const DebugSettings& debugSettings)
{
    const DebugSceneInformation& information = debugSettings.sceneInformation;
    DrawDebugText("SCENE / DISPLAY", panelBounds.x + 14, nextRowY, debugAccentColor);
    nextRowY += 25;

    DrawDebugRow(panelBounds, nextRowY, "Scene", information.sceneName);
    const std::string resourceCounts = TextFormat("%i / %i", static_cast<int>(information.modelCount),
                                                static_cast<int>(information.textureCount));
    DrawDebugRow(panelBounds, nextRowY, "Models / 2D tex", resourceCounts);
    DrawDebugRow(panelBounds, nextRowY, "Window", TextFormat("%i x %i", GetScreenWidth(), GetScreenHeight()));
    DrawDebugRow(panelBounds, nextRowY, "Render", TextFormat("%i x %i", information.renderWidth, information.renderHeight));

    std::string fpsLimitText = "unlimited";
    if (information.fpsLimit > 0)
    {
        fpsLimitText = TextFormat("%i", information.fpsLimit);
    }
    const char* vsyncText = information.isVsyncEnabled ? "on" : "off";
    DrawDebugRow(panelBounds, nextRowY, "VSync / FPS limit", TextFormat("%s / %s", vsyncText, fpsLimitText.c_str()));

    if (debugSettings.Show3DPosition)
    {
        const auto& cameraPosition = information.cameraPosition;
        const std::string cameraPositionText = TextFormat("%.1f, %.1f, %.1f",
                                                         cameraPosition[0], cameraPosition[1], cameraPosition[2]);
        DrawDebugRow(panelBounds, nextRowY, "Camera XYZ", cameraPositionText);
    }
    if (debugSettings.Show2DPosition)
    {
        DrawDebugRow(panelBounds, nextRowY, "Mouse XY", TextFormat("%i, %i", GetMouseX(), GetMouseY()));
    }
}

static void DrawDebugOptions(const Rectangle panelBounds, const Rectangle contentBounds,
                             const float nextRowY, const int columnCount, DebugSettings& debugSettings)
{
    struct ToggleOption
    {
        const char* label;
        bool* value;
    };
    const ToggleOption options[] = {
        {"Grid [H]", &debugSettings.Show3DGrid},
        {"FPS [F]", &debugSettings.ShowFps},
        {"Camera [P]", &debugSettings.Show3DPosition},
        {"Mouse [C]", &debugSettings.Show2DPosition},
        {"Frame graph", &debugSettings.ShowLatency},
        {"Console [F4]", &debugSettings.isConsoleVisible}
    };

    const bool isInteractive = CheckCollisionPointRec(GetMousePosition(), contentBounds);
    for (int optionIndex = 0; optionIndex < 6; ++optionIndex)
    {
        const int columnIndex = optionIndex % columnCount;
        const int rowIndex = optionIndex / columnCount;
        const float positionX = panelBounds.x + 14 + static_cast<float>(columnIndex) * panelBounds.width / 2;
        const float positionY = nextRowY + static_cast<float>(rowIndex * 27);
        const ToggleOption& option = options[optionIndex];
        DrawDebugToggle(positionX, positionY, option.label, *option.value, isInteractive);
    }
}

static void DrawDebugPanelHeader(const Rectangle panelBounds, DebugSettings& debugSettings)
{
    DrawDebugPanel(panelBounds, "LITE DEBUGGER");
    if (!debugSettings.isExpanded)
    {
        return;
    }

    const Rectangle resetButtonBounds = {panelBounds.x + panelBounds.width - 76, panelBounds.y + 7, 62, 25};
    if (DrawDebugButton(resetButtonBounds, "Reset"))
    {
        debugSettings.frameStatistics.Reset();
        debugSettings.memoryStatistics.ResetPeaks();
    }
}

void UpdateDebugGUI(DebugSettings& debugSettings)
{
    if (GetScreenWidth() < 220 || GetScreenHeight() < 140)
    {
        return;
    }

    const Rectangle panelBounds = GetDebugPanelBounds(debugSettings);
    DrawDebugPanelHeader(panelBounds, debugSettings);

    const Rectangle contentBounds = {panelBounds.x + 1, panelBounds.y + 44, panelBounds.width - 2,
                                     std::max(0.0f, panelBounds.height - 68)};
    const int toggleColumnCount = panelBounds.width >= 380 ? 2 : 1;
    const float contentHeight = GetDebugContentHeight(debugSettings, toggleColumnCount);
    UpdateDebugPanelScroll(contentBounds, contentHeight, debugSettings.isExpanded);

    BeginDebugClip(contentBounds);
    float nextRowY = contentBounds.y + 6;
    if (debugSettings.isExpanded)
    {
        nextRowY -= detailsScrollOffset;
    }
    DrawDebugPerformanceRows(panelBounds, nextRowY, debugSettings);
    if (debugSettings.isExpanded)
    {
        if (debugSettings.ShowLatency)
        {
            const Rectangle graphBounds = {panelBounds.x + 14, nextRowY + 4, panelBounds.width - 28, 68};
            DrawDebugFrameGraph(graphBounds, debugSettings.frameStatistics);
            nextRowY += 84;
        }
        nextRowY += 8;
        DrawDebugSceneRows(panelBounds, nextRowY, debugSettings);
        nextRowY += 10;
        DrawDebugOptions(panelBounds, contentBounds, nextRowY, toggleColumnCount, debugSettings);
    }
    EndScissorMode();

    const char* shortcutHint = "F3: hide | Shift+F3: tools | F4: log";
    if (debugSettings.isExpanded)
    {
        shortcutHint = "Shift+F3: compact | wheel: scroll";
    }
    DrawDebugText(shortcutHint, panelBounds.x + 14, panelBounds.y + panelBounds.height - 19, debugMutedColor);

    if (debugSettings.isExpanded && debugSettings.isConsoleVisible)
    {
        const Rectangle consoleBounds = GetDebugConsoleBounds(panelBounds);
        if (consoleBounds.height >= 120)
        {
            DrawDebugConsole(consoleBounds, debugSettings);
        }
    }
}
