#include <raylib.h>
#include "CameraHeader.h"
#include "ResourceManager.h"
#include <ShaderManager.h>
#include <Lighting.h>
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
void DrawGame(CameraManager &cameraManager, ResourceManager &rm) {
    BeginDrawing();
    ClearBackground(BLACK);
    Vector3 cubePosition = Vector3(0, 0, 0);
    BeginMode3D(cameraManager.camera);
    if (DebugSettings::Get().Show3DGrid)
        DrawGrid(100,10);
    BeginShader(cameraManager.camera);
    Light sun(LIGHT_DIRECTIONAL,Vector3(10, 2, 0), Vector3(0, 0, 0), WHITE,LightIntensity);
    rm.barrel.materials[0].shader = ShaderManager::GetInstance().shader;
    rm.barrel.materials[1].shader = ShaderManager::GetInstance().shader;
    rm.barrel.materials[2].shader = ShaderManager::GetInstance().shader;
    rm.barrel.materials[3].shader = ShaderManager::GetInstance().shader;
    DrawModel(rm.barrel, Vector3(0,5,0), 20, WHITE);

    //DrawCube(Vector3(0, 5, 0),10,10,10,RED);
    //SetMaterialColor(WHITE);
    DrawPlane(cubePosition,Vector2(100,100),BLUE);
    //SetMaterialColor(WHITE);
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
