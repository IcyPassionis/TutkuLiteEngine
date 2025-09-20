#pragma once

#ifndef LITEDEBUGGER_H
#define LITEDEBUGGER_H

struct DebugSettings { // Debug setting struct
    bool inDebugMode; // Checks if in debug mode or not
    bool ShowFps; // Shows FPS or no
    bool ShowLatency; // Show ms latency
    bool ShowProfiler; // Shows full profiler.
    bool Show3DGrid; // Shows 3D Grid
    bool Show3DColliders; // Shows collider box in 3D Space
    bool Show3DPosition; // Shows 3D Position of camera
    bool Show2DPosition; // Shows 2D Position of mouse
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
        ShowLatency = false;
        ShowProfiler = false;
        Show3DGrid = true;
        Show3DColliders = false;
        Show3DPosition = true;
        Show2DPosition = true;
    }
};

// Functions
void UpdateDebug(); // Updates all debug-related things
void CheckDebugKeys(DebugSettings& debugSettings); // Checks if any of debug keys has pressed and updates debug settings accordingly
void UpdateDebugGUI(DebugSettings& debugSettings); // Updates debug GUI
#endif
