#include "ShaderManager.h"
void SetMaterialColor(Color materialColor) {
    const ShaderManager &shaderManager = ShaderManager::GetInstance();
    Vector3 normalizedColor = {materialColor.r / 255.0f, materialColor.g / 255.0f, materialColor.b / 255.0f}; // Normalizes color for shader
    SetShaderValue(shaderManager.shader, shaderManager.materialColorLoc, &normalizedColor, SHADER_UNIFORM_VEC3);
}