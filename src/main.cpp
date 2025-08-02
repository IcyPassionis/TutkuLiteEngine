
#include <raylib.h>
#include <CameraHeader.h>
#include <thread>
#include <Settings.hpp>

#include "DrawGame.h"
#include "ResourceManager.h"
#include "../internal_libs/Lighting/include/ShaderManager.h"
#include "TimeState.h"
#include <LiteDebugger.h>

static TimeState timeState;

void InitGame();
void InitializeSingletons();
int main()
{
    InitGame();
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
void InitGame()
{
    Settings::Get();
    InitDraw();
    if (Settings::Get().isFpsLocked)
        SetTargetFPS(Settings::Get().fps);
    InitializeSingletons();
    StartScene();
    timeState.FixedThread = std::thread(&TimeState::FixedUpdateThread, &timeState);
}
void InitializeSingletons() {
    CameraManager::Get();
    ResourceManager::Get();
    ShaderManager::Get();
    DebugSettings::Get();
}
