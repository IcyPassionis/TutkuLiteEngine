#include "LiteDebugger.h"
#include "raygui.h"
#include <raylib.h>
#include <string>

void UpdateDebug() {
    DebugSettings& debugSettings = DebugSettings::Get();
    CheckDebugKeys(debugSettings);
    if (debugSettings.inDebugMode) {
        UpdateDebugGUI(debugSettings);
    }
}
void CheckDebugKeys(DebugSettings& debugSettings) {
    if (IsKeyPressed(KEY_F3)) {
        debugSettings.inDebugMode = !debugSettings.inDebugMode;
    }
    if (debugSettings.inDebugMode) {
        if (IsKeyPressed(KEY_H)) {
            debugSettings.Show3DGrid = !debugSettings.Show3DGrid;
        }
        if (IsKeyPressed(KEY_F)) {
            debugSettings.ShowFps = !debugSettings.ShowFps;
        }
    }
}
void UpdateDebugGUI(DebugSettings& debugSettings) {
    if (debugSettings.ShowFps) {
        std::string fps = "Fps: " + std::to_string(GetFPS());
        GuiTextBox(Rectangle(20,20,50,50),fps.data() , 30, false);
    }
}
