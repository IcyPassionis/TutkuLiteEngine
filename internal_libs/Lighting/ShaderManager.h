#pragma once

#ifndef SHADERMANAGER_H
#define SHADERMANAGER_H
#include <raylib.h>

struct ShaderManager {
    Shader shader; // Loaded main shader
    bool isInitialized; // Checks if shaderManager is initialized or not

    ShaderManager(const ShaderManager&) = delete;
    ShaderManager& operator=(const ShaderManager&) = delete;
    static ShaderManager& Get() { // Gets the instance of shaderManager singleton
        static ShaderManager instance;
        return instance;
    }

    // Friend Functions
    friend void BeginShader(Camera &camera); // Shader operation begins, this function always calls at drawGame Method
    friend void EndShader(); // Shader operations stop, this function always calls at drawGameMethod
    friend void SetMaterialColor(Color materialColor); // Sets shader's material color
    friend void SetShininess(float shininess); // Sets shader's shininess
    friend void ReloadShaders(); // Reload shaders
private:
    int materialColorLoc; // Material color location in the shader program
    int shininessLoc; // Shininess location in the shader program
    int ambientLoc; // Ambient color location in the shader program
    int ambientStrengthLoc; // Ambient strength location in the shader program
    ShaderManager() {
        LoadShaders();
    }
    ~ShaderManager() {
        UnloadShaders();
    }
    void LoadShaders() { // Loads shaders
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
    void UnloadShaders() {
        UnloadShader(shader);
        isInitialized = false;
    }

};
// Draw shaders method
void BeginShader(Camera &camera); // Shader operation begins, this function always calls at drawGame after 3d mode has been started
void EndShader(); // Shader operations stop, this function always calls at drawGameMethod

void ReloadShaders(); // Reload shaders
#endif