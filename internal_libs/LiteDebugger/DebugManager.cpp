#include "LiteDebugger.h"
#include "raygui.h"
#include <raylib.h>
void UpdateDebugKeys() {
    DebugSettings& debugSettings = DebugSettings::Get();
    if (IsKeyDown(KEY_F3)) {
        debugSettings.inDebugMode = !debugSettings.inDebugMode;
    }
    if (debugSettings.inDebugMode) {
        if (IsKeyDown(KEY_H) ) {
            debugSettings.Show3DGrid = !debugSettings.Show3DGrid;
        }
    }
}
