#define RAYGUI_IMPLEMENTATION
#include "raygui.h"
#include "LiteDebugger.h"
#include <raylib.h>
#include <string>

#include "../../include/CameraHeader.h"

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
        debugSettings.Show3DGrid = !debugSettings.Show3DGrid;
    }
    if (debugSettings.inDebugMode) {
        if (IsKeyPressed(KEY_H)) {
            debugSettings.Show3DGrid = !debugSettings.Show3DGrid;
        }
        if (IsKeyPressed(KEY_F)) {
            debugSettings.ShowFps = !debugSettings.ShowFps;
        }
        if (IsKeyPressed(KEY_P)) {
            debugSettings.Show3DPosition = !debugSettings.Show3DPosition;
        }
        if (IsKeyPressed(KEY_C)) {
            debugSettings.Show2DPosition = !debugSettings.Show2DPosition;
            if (debugSettings.Show2DPosition) {
                EnableCursor();
            }
            else {
                DisableCursor();
            }
        }
    }
}
void UpdateDebugGUI(DebugSettings& debugSettings) {
    if (debugSettings.ShowFps) {
        std::string fps = "Current Fps: " + std::to_string(GetFPS());
        GuiTextBox(Rectangle(20,20,150,50),fps.data() , 30, false);
    }
    if (debugSettings.Show3DPosition) {
        std::string position = "Camera Position: " + std::to_string(CameraManager::Get().camera.position.x) + " " + std::to_string(CameraManager::Get().camera.position.y) + " " + std::to_string(CameraManager::Get().camera.position.z);
       //std::string position = "Camera Position: ";
            GuiTextBox(Rectangle(20,80,400,50),position.data() , 30, false);
    }
    if (debugSettings.Show2DPosition) {
        std::string position = "Mouse Position: " + std::to_string(GetMouseX()) + " " + std::to_string(GetMouseY());
        GuiTextBox(Rectangle(20,140,150,50),position.data() , 30, false);
    }
}
