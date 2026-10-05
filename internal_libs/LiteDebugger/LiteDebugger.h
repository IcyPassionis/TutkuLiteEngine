#pragma once

#ifndef LITEDEBUGGER_H
#define LITEDEBUGGER_H

#include <array>
#include <cstddef>
#include <cstdint>
#include <string>
#include <vector>

inline constexpr std::size_t maximumDebugLogLines = 512;
inline constexpr std::size_t maximumDebugLogLineLength = 512;

struct DebugFrameStatistics
{
    std::array<float, 240> frameTimes = {};
    std::size_t frameCount = 0;
    std::size_t nextFrameIndex = 0;
    float currentFps = 0;
    float averageFps = 0;
    float minimumFps = 0;
    float maximumFps = 0;
    float frameMilliseconds = 0;

    void AddFrame(float frameSeconds);
    void Reset();
};

struct DebugMemoryStatistics
{
    std::uint64_t ramBytes = 0;
    std::uint64_t peakRamBytes = 0;
    std::uint64_t vramBytes = 0;
    std::uint64_t peakVramBytes = 0;
    bool isRamAvailable = false;
    bool isVramAvailable = false;

    void Sample();
    void ResetPeaks();
};

// Scope this in main, before initialization; restores the streams on every exit path.
struct DebugConsoleCapture
{
    DebugConsoleCapture();
    ~DebugConsoleCapture();
    DebugConsoleCapture(const DebugConsoleCapture&) = delete;
    DebugConsoleCapture& operator=(const DebugConsoleCapture&) = delete;
};

std::vector<std::string> GetDebugLogLines();
void ClearDebugLog();

struct DebugSceneInformation
{
    std::string sceneName;
    std::size_t modelCount = 0;
    std::size_t textureCount = 0;
    std::array<float, 3> cameraPosition = {};
    int renderWidth = 0;
    int renderHeight = 0;
    int fpsLimit = 0;
    bool isVsyncEnabled = false;
};

struct DebugSettings
{
    bool inDebugMode = true;
    bool ShowFps = true;
    bool ShowLatency = true;
    bool ShowProfiler = false;
    bool Show3DGrid = false;
    bool Show3DColliders = false;
    bool Show3DPosition = true;
    bool Show2DPosition = false;

    bool isExpanded = false;
    bool isConsoleVisible = true;
    bool isConsoleFollowing = true;
    bool isCursorPreviouslyEnabled = false;

    DebugFrameStatistics frameStatistics;
    DebugMemoryStatistics memoryStatistics;
    DebugSceneInformation sceneInformation;

    DebugSettings(const DebugSettings&) = delete;
    DebugSettings& operator=(const DebugSettings&) = delete;
    static DebugSettings& Get()
    {
        static DebugSettings instance;
        return instance;
    }
private:
    DebugSettings() = default;
};

void UpdateDebug();
void UpdateDebugInput(); // Call before camera movement and 3D drawing.
void CheckDebugKeys(DebugSettings& debugSettings);
void UpdateDebugGUI(DebugSettings& debugSettings);
#endif
