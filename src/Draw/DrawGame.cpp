#include "DrawGame.h"

#include "CameraHeader.h"
#include "LiteDebugger.h"
#include "ResourceManager.h"
#include "Settings.hpp"
#include "SceneManager.h"
#include "States.h"
#include "Rendering/Renderer3D.h"

#include <r3d/r3d.h>
#include <raylib.h>
#include <iostream>

static WindowStates ws;
static R3D_Mesh defaultPlane = {};
static R3D_Material defaultPlaneMaterial = {};
static R3D_Light defaultDirectionalLight = 0;
static bool defaultObjectsCreated = false;

void InitDraw() {
  unsigned int configFlags = FLAG_MSAA_4X_HINT | FLAG_WINDOW_RESIZABLE;
  if (Settings::Get().isVsyncEnabled) configFlags |= FLAG_VSYNC_HINT;
  SetConfigFlags(configFlags);
  InitWindow(ws.width, ws.height, ws.title.c_str());
  SetWindowMonitor(ws.currentMonitor);
  DisableCursor();
}

void LoadScenes() {}

static void UpdateDebugSceneInformation(const Camera3D& camera, const int renderWidth, const int renderHeight)
{
    DebugSettings& debugSettings = DebugSettings::Get();
    if (!debugSettings.isExpanded)
    {
        return;
    }

    SceneManager& sceneManager = SceneManager::Get();
    const std::lock_guard<std::mutex> lock(sceneManager.sceneMutex);
    const Scene* const currentScene = sceneManager.GetCurrentScene();
    const ResourceManager& resourceManager = ResourceManager::Get();
    const Settings& settings = Settings::Get();
    DebugSceneInformation& information = debugSettings.sceneInformation;

    information.sceneName = "Default scene";
    if (currentScene != nullptr && currentScene->name != "empty")
    {
        information.sceneName = currentScene->name;
    }
    information.modelCount = resourceManager.currentLoadedModels.size();
    information.textureCount = resourceManager.currentLoaded2DTextures.size();
    information.cameraPosition = {camera.position.x, camera.position.y, camera.position.z};
    information.renderWidth = renderWidth;
    information.renderHeight = renderHeight;
    information.isVsyncEnabled = settings.isVsyncEnabled;
    information.fpsLimit = settings.isFpsLocked ? settings.fps : 0;
}

void DrawGame() {
  CameraManager& cameraManager = CameraManager::Get();
  Renderer3D& renderer = Renderer3D::Get();
  const int width = GetScreenWidth();
  const int height = GetScreenHeight();
  if (width > 0 && height > 0) renderer.Resize(width, height);

  BeginDrawing();
  ClearBackground(BLACK);
  renderer.Begin(cameraManager.camera);
  DrawScene();
  renderer.End();

  if (DebugSettings::Get().Show3DGrid) {
    BeginMode3D(cameraManager.camera);
    DrawGrid(100, 10);
    EndMode3D();
  }
  UpdateDebugSceneInformation(cameraManager.camera, width, height);
  UpdateDebug();
  EndDrawing();
}

void DrawScene() {
  SceneManager& sceneManager = SceneManager::Get();
  std::lock_guard<std::mutex> lock(sceneManager.sceneMutex);
  Scene* currentScene = sceneManager.GetCurrentScene();
  if (currentScene == nullptr || currentScene->name == "empty") {
    DrawDefaultScene();
  } else {
    currentScene->DrawScene();
  }
}

void PlaceDefaultObjects() {
  if (defaultObjectsCreated || !Renderer3D::Get().IsInitialized()) return;
  ResourceManager::Get();
  defaultObjectsCreated = true;
  defaultDirectionalLight = R3D_CreateLight(R3D_LIGHT_DIR);
  R3D_SetLightDirection(defaultDirectionalLight, (Vector3){0.7f, -1.0f, 0.7f});
  R3D_SetLightColor(defaultDirectionalLight, WHITE);
  R3D_SetLightEnergy(defaultDirectionalLight, 1.0f);
  R3D_EnableLight(defaultDirectionalLight);
  R3D_EnableShadow(defaultDirectionalLight);
  defaultPlane = R3D_GenMeshPlane(100.0f, 100.0f, 1, 1);
  defaultPlaneMaterial = R3D_GetDefaultMaterial();
}

void DrawDefaultScene() {
  PlaceDefaultObjects();
  ResourceManager& resources = ResourceManager::Get();
  const auto barrel = resources.currentLoadedModels.find("barrel");
  if (barrel != resources.currentLoadedModels.end()) {
    R3D_DrawModelEx(barrel->second, (Vector3){0, 5, 0}, QuaternionIdentity(),
                    (Vector3){10, 10, 10});
  }
  if (defaultObjectsCreated) {
    R3D_DrawMesh(defaultPlane, defaultPlaneMaterial, (Vector3){0, 0, 0}, 1.0f);
  }
}

void ShutdownDraw() {
  if (defaultDirectionalLight != 0 && Renderer3D::Get().IsInitialized()) {
    R3D_DestroyLight(defaultDirectionalLight);
    defaultDirectionalLight = 0;
  }
  if (defaultObjectsCreated && Renderer3D::Get().IsInitialized()) {
    R3D_UnloadMesh(defaultPlane);
    defaultPlane = {};
    defaultObjectsCreated = false;
  }
  ResourceManager::Get().Shutdown();
}
