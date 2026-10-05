#include "LiteDebugger.h"

#include <algorithm>
#include <cmath>

static float CalculateRecentAverageFps(const DebugFrameStatistics& statistics)
{
    float elapsedSeconds = 0;
    std::size_t recentFrameCount = 0;
    const std::size_t sampleCapacity = statistics.frameTimes.size();
    while (recentFrameCount < statistics.frameCount && elapsedSeconds < 3.0f)
    {
        const std::size_t sampleIndex = (statistics.nextFrameIndex + sampleCapacity - 1 - recentFrameCount) % sampleCapacity;
        elapsedSeconds += statistics.frameTimes[sampleIndex] / 1000.0f;
        ++recentFrameCount;
    }
    return static_cast<float>(recentFrameCount) / elapsedSeconds;
}

void DebugFrameStatistics::AddFrame(const float frameSeconds)
{
    if (!std::isfinite(frameSeconds) || frameSeconds <= 0)
    {
        return;
    }

    currentFps = 1.0f / frameSeconds;
    frameMilliseconds = frameSeconds * 1000.0f;
    if (frameCount == 0)
    {
        minimumFps = currentFps;
    }
    else
    {
        minimumFps = std::min(minimumFps, currentFps);
    }
    maximumFps = std::max(maximumFps, currentFps);

    frameTimes[nextFrameIndex] = frameMilliseconds;
    nextFrameIndex = (nextFrameIndex + 1) % frameTimes.size();
    frameCount = std::min(frameCount + 1, frameTimes.size());
    averageFps = CalculateRecentAverageFps(*this);
}

void DebugFrameStatistics::Reset()
{
    *this = DebugFrameStatistics{};
}
