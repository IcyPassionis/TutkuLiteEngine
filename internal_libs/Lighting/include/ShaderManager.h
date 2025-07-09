#pragma once

#ifndef SHADERMANAGER_H
#define SHADERMANAGER_H
#include <raylib.h>
#include <vector>
struct Light;

struct ShaderManager {
    Shader shader; // Loaded main shader
    bool isInitialized; // Checks if shaderManager is initialized or not
    std::vector<Light> lights;
    RenderTexture2D shadowMap; // Shadow map texture
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
    void LoadShaders();// Loads shaders

    void UnloadShaders(); // Unload shader

    void ReloadLights(); // Reload every Light to use the current shader.

    void UpdateShadowMap();

};
// Draw shaders method
void BeginShader(Camera &camera); // Shader operation begins, this function always calls at drawGame after 3d mode has been started
void EndShader(); // Shader operations stop, this function always calls at drawGameMethod

void ReloadShaders(); // Reload shaders
#endif