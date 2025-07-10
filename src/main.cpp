#define RAYGUI_IMPLEMENTATION
#include "raygui.h"
#include <raylib.h>
#include <CameraHeader.h>
#include <thread>
#include <Settings.hpp>

#include "DrawGame.h"
#include "ResourceManager.h"
#include "../internal_libs/Lighting/include/ShaderManager.h"
#include "TimeState.h"
#include <LiteDebugger.h>

inline Settings settings;
static TimeState timeState;
void InitializeSingletons();
int main()
{
    InitDraw();
    if (settings.isFpsLocked)
        SetTargetFPS(settings.fps);
    InitializeSingletons();
    StartScene();
    std::thread FixedThread(&TimeState::FixedUpdateThread, &timeState);
    while (!WindowShouldClose()) {
        UpdateCamera(&CameraManager::Get().camera, CAMERA_FREE);
        DrawGame();
        if (IsKeyDown(KEY_LEFT_CONTROL) && IsKeyPressed(KEY_R))
            ReloadShaders();
        UpdateDebug();
        timeState.UpdateDeltaTime();
    }
    CloseWindow();
}
void InitializeSingletons() {
    CameraManager& cameraManager = CameraManager::Get();
    ResourceManager& resourceManager = ResourceManager::Get();
    ShaderManager& shaderManager = ShaderManager::Get();
    DebugSettings& debugSettings = DebugSettings::Get();
}
