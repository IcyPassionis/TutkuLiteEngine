#pragma once
#ifndef DEBUGDRAWING_H
#define DEBUGDRAWING_H

#include <raylib.h>
#include <string>

struct DebugSettings;

inline constexpr Color debugPanelColor = {17, 22, 32, 235};
inline constexpr Color debugBorderColor = {65, 76, 94, 255};
inline constexpr Color debugTextColor = {224, 232, 242, 255};
inline constexpr Color debugMutedColor = {149, 164, 183, 255};
inline constexpr Color debugAccentColor = {129, 205, 215, 255};

void DrawDebugText(const char* text, float positionX, float positionY, Color color = debugTextColor);
bool DrawDebugButton(Rectangle bounds, const char* label);
void DrawDebugToggle(float positionX, float positionY, const char* label, bool& value, bool isInteractive);
void DrawDebugPanel(Rectangle bounds, const char* title);
void DrawDebugRow(Rectangle bounds, float& nextRowY, const char* label, const std::string& value);
void BeginDebugClip(Rectangle bounds);
void DrawDebugConsole(Rectangle bounds, DebugSettings& debugSettings);

#endif
