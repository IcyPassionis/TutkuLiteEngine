#include "ShaderManager.h"
void SetMaterialColor(Color materialColor) {
    const ShaderManager &shaderManager = ShaderManager::GetInstance(); // Gets shader manager instance
    Vector3 normalizedColor = {materialColor.r / 255.0f, materialColor.g / 255.0f, materialColor.b / 255.0f}; // Normalizes color for shader
    SetShaderValue(shaderManager.shader, shaderManager.materialColorLoc, &normalizedColor, SHADER_UNIFORM_VEC3);
}
void BeginShader(Camera &camera) {
    ShaderManager &shaderManager = ShaderManager::GetInstance(); // Gets shader manager instance
    SetShaderValue(shaderManager.shader, shaderManager.shader.locs[SHADER_LOC_VECTOR_VIEW], &camera.position, SHADER_UNIFORM_VEC3); // Sets shader's viewPos variable to camera's position
    BeginShaderMode(shaderManager.shader);
}
void EndShader() {
    EndShaderMode();
}
void SetShininess(const float shininess) { // Sets shader's shininess
    ShaderManager &shaderManager = ShaderManager::GetInstance(); // Gets shader manager instance
    SetShaderValue(shaderManager.shader, shaderManager.shininessLoc, &shininess, SHADER_UNIFORM_FLOAT);
}
void ReloadShaders() {
    ShaderManager &shaderManager = ShaderManager::GetInstance(); // Gets shader manager instance
    UnloadShader(shaderManager.shader); // Unloads shader
    shaderManager.LoadShaders(); // Loads shader
}