#include "ShaderManager.h"
#include "Lighting.h"
#include "../../../include/DrawGame.h"

void ShaderManager::LoadShaders() {
    shader = LoadShader(SHADERS_PATH "glsl100/lighting.vert",SHADERS_PATH "glsl100/lighting.frag");
    ambientLoc = GetShaderLocation(shader, "ambientColor"); // Ambient Color location in the shader program
    ambientStrengthLoc = GetShaderLocation(shader, "ambientStrength"); // Ambient strength location in the shader program
    materialColorLoc = GetShaderLocation(shader, "materialColor"); // Material color location in the shader program
    shininessLoc = GetShaderLocation(shader, "shininess"); // Shininess location in the shader program
    float a = 0.25f;
    SetShaderValue(shader, ambientStrengthLoc, &a, SHADER_UNIFORM_FLOAT); // Sets shader's ambient strength
    float ambientColor[3] = {0.3f, 0.4f, 0.6f};
    SetShaderValue(shader, ambientLoc, ambientColor , SHADER_UNIFORM_VEC3); // Sets shader's ambient color
    shader.locs[SHADER_LOC_VECTOR_VIEW] = GetShaderLocation(shader, "viewPos");

    isInitialized = true;
}
void ShaderManager::UnloadShaders() {
    UnloadShader(shader);
    isInitialized = false;
}

void ShaderManager::ReloadLights() {
    if (lights.size() > 0) {
        for (int i = 0; i < lights.size(); i++) {
            lights[i].ReloadLight();
        }
    }
}

void ShaderManager::UpdateShadowMap() {
    BeginTextureMode(shadowMap);
    BeginMode3D(lights[0].camera);
    DrawScene();
    EndMode3D();
    EndTextureMode();
}

