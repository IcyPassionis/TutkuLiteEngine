#include "DrawGame.h"

#include <raylib.h>
#include "CameraHeader.h"
#include "ResourceManager.h"
#include <ShaderManager.h>
#include "LiteDebugger.h"
#include "Settings.hpp"

#include "SceneManager.h"
#include "States.h"
static WindowStates ws;
float Shininess = 0;
float LightIntensity = 5;
void InitDraw() {
    SetConfigFlags(FLAG_MSAA_4X_HINT);
    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    if (Settings::Get().isVsyncEnabled)
        SetConfigFlags(FLAG_VSYNC_HINT);
    InitWindow(ws.width,ws.height,ws.title.c_str());
    SetWindowMonitor(ws.currentMonitor);
    DisableCursor();
}
void StartScene() {
    ShaderManager::Get().lights.emplace_back(LIGHT_DIRECTIONAL, Vector3(0, 0, 0), Vector3(15, -2, 15), WHITE, 1);
}

void DrawGame() {
    CameraManager& cameraManager = CameraManager::Get();
    BeginDrawing();
    ClearBackground(BLACK);
    BeginMode3D(cameraManager.camera);
    if (DebugSettings::Get().Show3DGrid)
        DrawGrid(100,10);
    BeginShader(cameraManager.camera);
    DrawScene();
    if (IsKeyDown(KEY_LEFT)) {
        LightIntensity--;
        std::cout << "MAIN THREAD: " << "Light Intensity: " << LightIntensity << "\n";
    }
    else if (IsKeyDown(KEY_RIGHT)) {
        LightIntensity++;
        std::cout << "MAIN THREAD: " << "Light Intensity: " << LightIntensity << "\n";
    }
    EndShader();
    EndMode3D();
    EndDrawing();
}
void DrawScene() {
  SceneManager &sm = SceneManager::Get();
  Scene currentScene = sm.LoadScene(sm.currentSceneID);
  if (currentScene.name == "empty") // REVERTS TO DEFAULT SCENE
  {
    DrawDefaultScene();
  }
  else // DRAWS CURRENT SCENE !
  {
    currentScene.DrawScene();
  }
}
void DrawDefaultScene ()
{
  ResourceManager &rm = ResourceManager::Get();
  rm.LoadShadersToModels();
  DrawModel(rm.currentLoadedModels["barrel"], Vector3(0, 5, 0), 10, Color(255, 255, 255));
  //DrawSceneGeometry();
}
