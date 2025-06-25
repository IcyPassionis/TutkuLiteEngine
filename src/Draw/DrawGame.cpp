#include <raylib.h>
#include "CameraHeader.h"
#include "ResourceManager.h"
#include <ShaderManager.h>
#include <Lighting.h>
#include "States.h"
static WindowState ws = settings;
float Shininess = 1;
float LightIntensity = 1;
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
    DrawGrid(100,10);
    BeginShader(cameraManager.camera);
    Light sun(LIGHT_DIRECTIONAL,Vector3(0, 50, 0), Vector3(-0.5f, -1.0f, -0.5f), WHITE,LightIntensity);
    //DrawModel(rm.barrel, cubePosition, 20, WHITE);
    DrawCube(Vector3(0, 3, 0),10,10,10,WHITE);
    SetMaterialColor(WHITE);
    DrawPlane(cubePosition,Vector2(100,100),RED);
    SetMaterialColor(WHITE);
    if (IsKeyDown(KEY_LEFT)) {
        LightIntensity--;
    }
    else if (IsKeyDown(KEY_RIGHT)) {
        LightIntensity++;
    }
    if (IsKeyDown(KEY_UP)) {
        Shininess++;
    }
    else if (IsKeyDown(KEY_DOWN)) {
        Shininess--;
    }
    SetShininess(Shininess);
    EndShader();
    EndMode3D();
    EndDrawing();
}
