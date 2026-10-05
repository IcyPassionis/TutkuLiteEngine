#define RAYGUI_IMPLEMENTATION
#include "DebugDrawing.h"

#include <raygui.h>

#include <algorithm>

void DrawDebugText(const char* text, const float positionX, const float positionY, const Color color)
{
    DrawTextEx(GetFontDefault(), text, {positionX, positionY}, 16, 1, color);
}

bool DrawDebugButton(const Rectangle bounds, const char* label)
{
    const bool isHovered = CheckCollisionPointRec(GetMousePosition(), bounds);
    const Color buttonColor = isHovered ? Color{50, 70, 87, 255} : Color{33, 43, 58, 255};
    DrawRectangleRec(bounds, buttonColor);
    DrawRectangleLinesEx(bounds, 1, debugBorderColor);

    const Vector2 textSize = MeasureTextEx(GetFontDefault(), label, 16, 1);
    const float textPositionX = bounds.x + (bounds.width - textSize.x) / 2;
    const float textPositionY = bounds.y + (bounds.height - textSize.y) / 2;
    DrawDebugText(label, textPositionX, textPositionY);

    return isHovered && IsMouseButtonReleased(MOUSE_BUTTON_LEFT);
}

void DrawDebugToggle(const float positionX, const float positionY, const char* label,
                     bool& value, const bool isInteractive)
{
    const Rectangle bounds = {positionX, positionY, 170, 22};
    const bool isHovered = CheckCollisionPointRec(GetMousePosition(), bounds);
    if (isInteractive && isHovered && IsMouseButtonReleased(MOUSE_BUTTON_LEFT))
    {
        value = !value;
    }

    const Color checkboxColor = value ? debugAccentColor : debugBorderColor;
    DrawRectangleRec({positionX, positionY + 2, 16, 16}, checkboxColor);
    if (value)
    {
        DrawDebugText("x", positionX + 4, positionY + 2, debugPanelColor);
    }
    DrawDebugText(label, positionX + 24, positionY + 3, debugMutedColor);
}

void DrawDebugPanel(const Rectangle bounds, const char* title)
{
    DrawRectangleRec(bounds, debugPanelColor);
    DrawRectangleLinesEx(bounds, 1, debugBorderColor);
    DrawDebugText(title, bounds.x + 14, bounds.y + 12, debugAccentColor);

    const Vector2 separatorStart = {bounds.x + 1, bounds.y + 38};
    const Vector2 separatorEnd = {bounds.x + bounds.width - 1, bounds.y + 38};
    DrawLineEx(separatorStart, separatorEnd, 1, debugBorderColor);
}

void DrawDebugRow(const Rectangle bounds, float& nextRowY, const char* label, const std::string& value)
{
    DrawDebugText(label, bounds.x + 14, nextRowY, debugMutedColor);

    const Vector2 valueSize = MeasureTextEx(GetFontDefault(), value.c_str(), 16, 1);
    const float valuePositionX = std::max(bounds.x + 150, bounds.x + bounds.width - 14 - valueSize.x);
    DrawDebugText(value.c_str(), valuePositionX, nextRowY);
    nextRowY += 24;
}

void BeginDebugClip(const Rectangle bounds)
{
    BeginScissorMode(static_cast<int>(bounds.x), static_cast<int>(bounds.y),
                     static_cast<int>(bounds.width), static_cast<int>(bounds.height));
}
