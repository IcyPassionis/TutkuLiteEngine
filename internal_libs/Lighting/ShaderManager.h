#pragma once

#ifndef SHADERMANAGER_H
#define SHADERMANAGER_H
#include <raylib.h>
struct ShaderManager {
    Shader shader; // Loaded main shader

    ShaderManager(const ShaderManager&) = delete;
    ShaderManager& operator=(const ShaderManager&) = delete;
    static ShaderManager& GetInstance() { // Gets the instance of shaderManager singleton
        static ShaderManager instance;
        return instance;
    }
    friend void BeginShader(Camera &camera); // Shader operation begins, this function always calls at drawGame Method
    friend void EndShader(); // Shader operations stop, this function always calls at drawGameMethod
    friend void SetMaterialColor(Color materialColor); // Sets shader's material color
    friend void SetShininess(float shininess); // Sets shader's shininess
private:
    int viewPosLoc; // viewPos is the camera's position in
    int materialColorLoc; // Material color location in the shader program
    int shininessLoc; // Shininess location in the shader program
    int ambientLoc; // Ambient color location in the shader program
    int ambientStrengthLoc; // Ambient strength location in the shader program
    ShaderManager() {
        shader = LoadShader(RESOURCES_PATH "shaders/glsl100/lighting.vert",RESOURCES_PATH "shaders/glsl100/lighting.frag");
        ambientLoc = GetShaderLocation(shader, "ambientColor"); // Ambient Color location in the shader program
        ambientStrengthLoc = GetShaderLocation(shader, "ambientStrength"); // Ambient strength location in the shader program
        materialColorLoc = GetShaderLocation(shader, "materialColor"); // Material color location in the shader program
        shininessLoc = GetShaderLocation(shader, "shininess"); // Shininess location in the shader program
        float a = 0.2f;
        SetShaderValue(shader, ambientStrengthLoc, &a, SHADER_UNIFORM_FLOAT); // Sets shader's ambient strength
        SetShaderValue(shader, ambientLoc, (float[3]){0.3f, 0.4f, 0.6f}, SHADER_UNIFORM_VEC3); // Sets shader's ambient color
        viewPosLoc = GetShaderLocation(shader, "viewPos"); // Gets viewPos location in the shader program
    }
    ~ShaderManager() {
        UnloadShader(shader);
    }

};
// Draw shaders method
void BeginShader(Camera &camera); // Shader operation begins, this function always calls at drawGame after 3d mode has been started
void EndShader(); // Shader operations stop, this function always calls at drawGameMethod

// Set Material value method
void SetMaterialColor(Color materialColor); // Sets shader's material color
void SetShininess(float shininess); // Sets shader's shininess
#endif