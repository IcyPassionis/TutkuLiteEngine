#include <raylib.h>
#include <CameraHeader.h>
#include <thread>
#include <Settings.hpp>

#include "DrawGame.h"
#include "ResourceManager.h"
#include "ShaderManager.h"
#include "TimeState.h"
inline Settings settings;
static TimeState timeState;
int main()
{
    InitDraw();
    if (settings.isFpsLocked)
        SetTargetFPS(settings.fps);
    CameraManager cameraManager;
    ResourceManager resourceManager;
    ShaderManager& shaderManager = ShaderManager::GetInstance();
    std::thread FixedThread(&TimeState::FixedUpdateThread, &timeState);
    while (!WindowShouldClose()) {
        UpdateCamera(&cameraManager.camera, CAMERA_FREE);
        DrawGame(cameraManager,resourceManager);
        timeState.UpdateDeltaTime();
    }
    CloseWindow();
}
