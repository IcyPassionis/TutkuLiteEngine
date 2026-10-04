#include <CameraHeader.h>
#include <Settings.hpp>
#include <raylib.h>
#include <thread>

#include "DrawGame.h"
#include "ResourceManager.h"
#include "TimeState.h"
#include "LiteDebugger.h"
#include "SceneManager.h"
#include "Rendering/Renderer3D.h"

static TimeState timeState;

void InitializeSingletons();
int main() {
  Settings::Get();
  InitDraw();
  if (!Renderer3D::Get().Init(GetScreenWidth(), GetScreenHeight())) {
    CloseWindow();
    return 1;
  }
  if (Settings::Get().isFpsLocked)
    SetTargetFPS(Settings::Get().fps);
  InitializeSingletons();
  LoadScenes();
  timeState.isRunning = true;
  timeState.FixedThread =
      std::thread(&TimeState::FixedUpdateThread, &timeState);

  while (!WindowShouldClose()) {
    UpdateCamera(&CameraManager::Get().camera, CAMERA_FREE);
    DrawGame();
    timeState.UpdateDeltaTime();
  }
  timeState.isRunning = false;
  if (timeState.FixedThread.joinable())
    timeState.FixedThread.join();
  ShutdownDraw();
  Renderer3D::Get().Close();
  CloseWindow();
}

void InitializeSingletons() {
  CameraManager::Get();
  SceneManager::Get();
  ResourceManager::Get();
  DebugSettings::Get();
  PlaceDefaultObjects();
}
