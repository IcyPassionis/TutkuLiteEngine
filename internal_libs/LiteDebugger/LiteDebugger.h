#pragma once

#ifndef LITEDEBUGGER_H
#define LITEDEBUGGER_H

struct DebugSettings { // Debug setting struct
    bool inDebugMode; // Checks if in debug mode or not
    bool ShowFps; // Shows FPS or not
    bool ShowProfiler; // Shows profiler or not(includes ms latency)
    bool Show3DGrid; // Shows 3D Grid
    bool Show3DColliders; // Shows collider box in 3D Space
    DebugSettings(const DebugSettings&) = delete;
    DebugSettings& operator=(const DebugSettings&) = delete;
    static DebugSettings& Get() {
        static DebugSettings instance;
        return instance;
    }
private:
    DebugSettings() {
        inDebugMode = true;
        ShowFps = true;
        ShowProfiler = false;
        Show3DGrid = false;
        Show3DColliders = false;
    }
};

// Functions
void UpdateDebugKeys(); // Checks if debug keys have been pressed
#endif
