#include "DrawGame.h"

#include "CameraHeader.h"
#include "LiteDebugger.h"
#include "ResourceManager.h"
#include "Settings.hpp"
#include <ShaderManager.h>
#include <raylib.h>

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
  InitWindow(ws.width, ws.height, ws.title.c_str());
  SetWindowMonitor(ws.currentMonitor);
  DisableCursor();
}

void LoadScenes()
{

}

void DrawSceneGeometry() {

  ResourceManager &rm = ResourceManager::Get();
  rm.LoadShadersToModels();
  DrawModel(rm.currentLoadedModels["barrel"], Vector3(0, 5, 0), 10, Color(255, 255, 255));
  DrawPlane(Vector3(0, 0, 0), Vector2(100, 100), BLUE);
}

void DrawGame() {
  CameraManager &cameraManager = CameraManager::Get();

  // Shadow pass — must happen before BeginDrawing
  ShaderManager::Get().UpdateShadowMap();

  BeginDrawing();
  ClearBackground(BLACK);
  BeginMode3D(cameraManager.camera);
  if (DebugSettings::Get().Show3DGrid)
    DrawGrid(100, 10);
  BeginShader(cameraManager.camera);
  DrawScene();
  if (IsKeyDown(KEY_LEFT)) {
    LightIntensity--;
    std::cout << "MAIN THREAD: " << "Light Intensity: " << LightIntensity
              << "\n";
  } else if (IsKeyDown(KEY_RIGHT)) {
    LightIntensity++;
    std::cout << "MAIN THREAD: " << "Light Intensity: " << LightIntensity
              << "\n";
  }
  EndShader();
  EndMode3D();
  UpdateDebug();
  EndDrawing();
}
void DrawScene() {
  SceneManager &sm = SceneManager::Get();
  Scene currentScene = sm.LoadScene(sm.currentSceneID);
  if (currentScene.name == "empty") // REVERTS TO DEFAULT SCENE
  {
    bool isFirstTime = false;
    if(!isFirstTime)  {
        PlaceDefaultObjects();
        isFirstTime = true;
    }
    DrawDefaultScene();

  }
  else // DRAWS CURRENT SCENE !
  {
    currentScene.DrawScene();
  }
}
void PlaceDefaultObjects()
{
  ShaderManager::Get().lights.emplace_back(LIGHT_DIRECTIONAL, Vector3(0, 20, 0),  Vector3(10, -4 , 10), WHITE, 1);
}
void DrawDefaultScene ()
{
  ResourceManager &rm = ResourceManager::Get();
  DrawModel(rm.currentLoadedModels["barrel"], Vector3(0, 5, 0), 10, Color(255, 255, 255));
  DrawPlane(Vector3(0, 0, 0), Vector2(100, 100), BLUE);
  //DrawSceneGeometry();
}
