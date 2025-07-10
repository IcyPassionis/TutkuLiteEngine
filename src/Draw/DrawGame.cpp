#include "DrawGame.h"

#include <raylib.h>
#include "CameraHeader.h"
#include "ResourceManager.h"
#include <ShaderManager.h>
#include "LiteDebugger.h"
#include "States.h"
static WindowState ws = settings;
float Shininess = 0;
float LightIntensity = 5;
void InitDraw() {
    SetConfigFlags(FLAG_MSAA_4X_HINT);
    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    InitWindow(ws.width,ws.height,ws.title.c_str());
    DisableCursor();
}
void StartScene() {
    ShaderManager::Get().lights.push_back(Light(LIGHT_DIRECTIONAL, Vector3(0, 0, 0), Vector3(15, -2, 15), WHITE, 1));
}

void DrawGame() {
    CameraManager& cameraManager = CameraManager::Get();
    ResourceManager& rm = ResourceManager::Get();
    BeginDrawing();
    ClearBackground(BLACK);
    BeginMode3D(cameraManager.camera);
    if (DebugSettings::Get().Show3DGrid)
        DrawGrid(100,10);
    BeginShader(cameraManager.camera);
    DrawScene();
    if (IsKeyDown(KEY_LEFT)) {
        LightIntensity--;
        std::cout << "Light Intensity: " << LightIntensity << "\n";
    }
    else if (IsKeyDown(KEY_RIGHT)) {
        LightIntensity++;
        std::cout << "Light Intensity: " << LightIntensity << "\n";
    }
    EndShader();
    EndMode3D();
    EndDrawing();
}
void DrawScene() {
    ResourceManager& rm = ResourceManager::Get();
    rm.barrel.materials[0].shader = ShaderManager::Get().shader;
    rm.barrel.materials[1].shader = ShaderManager::Get().shader;
    rm.barrel.materials[2].shader = ShaderManager::Get().shader;
    rm.barrel.materials[3].shader = ShaderManager::Get().shader;
    //DrawModel(rm.barrel, Vector3(0,5,0), 20, WHITE);
    Vector3 cubePosition = Vector3(0, 0, 0);
    DrawCube(Vector3(0, 5, 0),10,10,10,RED);
    DrawPlane(cubePosition,Vector2(100,100),BLUE);
}

