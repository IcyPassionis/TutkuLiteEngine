#include <iostream>

#include "ShaderManager.h"
#include "Lighting.h"
#include <rlgl.h>

void BeginShader(Camera &camera) {
    ShaderManager &shaderManager = ShaderManager::Get();
    SetShaderValue(shaderManager.shader, shaderManager.shader.locs[SHADER_LOC_VECTOR_VIEW], &camera.position, SHADER_UNIFORM_VEC3);
    rlActiveTextureSlot(1);
    rlEnableTexture(shaderManager.shadowMap.depth.id);
    BeginShaderMode(shaderManager.shader);
}

void EndShader() {
    EndShaderMode();
    rlActiveTextureSlot(1);
    rlDisableTexture();
    rlActiveTextureSlot(0);
}

void ReloadShaders() {
    ShaderManager &shaderManager = ShaderManager::Get();
    shaderManager.UnloadShaders();
    shaderManager.LoadShaders();
    shaderManager.ReloadLights();
    std::cout << "INFO: Shaders Reloaded\n";
}
