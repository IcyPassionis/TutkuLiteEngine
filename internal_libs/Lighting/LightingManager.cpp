#include "ShaderManager.h"

void BeginShader(Camera &camera) {
    ShaderManager &shaderManager = ShaderManager::Get(); // Gets shader manager instance
    SetShaderValue(shaderManager.shader, shaderManager.shader.locs[SHADER_LOC_VECTOR_VIEW], &camera.position, SHADER_UNIFORM_VEC3); // Sets shader's viewPos variable to camera's position
    BeginShaderMode(shaderManager.shader);
}
void EndShader() {
    EndShaderMode();
}

void ReloadShaders() {
    ShaderManager &shaderManager = ShaderManager::Get(); // Gets shader manager instance
    shaderManager.UnloadShaders(); // Unloads shader
    shaderManager.LoadShaders(); // Loads shader
}