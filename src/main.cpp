#define RAYGUI_IMPLEMENTATION
#include "raygui.h"
#include <raylib.h>
#include <CameraHeader.h>
#include <thread>
#include <Settings.hpp>

#include "DrawGame.h"
#include "ResourceManager.h"
#include "ShaderManager.h"
#include "TimeState.h"
#include <LiteDebugger.h>

inline Settings settings;
static TimeState timeState;
int main()
{
    InitDraw();
    if (settings.isFpsLocked)
        SetTargetFPS(settings.fps);
    CameraManager& cameraManager = CameraManager::Get();
    ResourceManager& resourceManager = ResourceManager::Get();
    DebugSettings& debugSettings = DebugSettings::Get();
    ShaderManager& shaderManager = ShaderManager::GetInstance();
    resourceManager.lights.push_back(Light(LIGHT_DIRECTIONAL, Vector3(0, 0, 0), Vector3(15, -2, 15), WHITE, 1));
    std::thread FixedThread(&TimeState::FixedUpdateThread, &timeState);
    while (!WindowShouldClose()) {
        UpdateCamera(&cameraManager.camera, CAMERA_FREE);
        DrawGame(cameraManager,resourceManager);
        UpdateDebug();
        timeState.UpdateDeltaTime();
    }
    CloseWindow();
}
