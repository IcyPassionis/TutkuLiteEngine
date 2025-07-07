#pragma once

#ifndef LIGHTING_H
#define LIGHTING_H
#define MAX_LIGHTS 20
#include <raylib.h>
#include <ShaderManager.h>
enum TypeOfLight { // Lights type enum
    LIGHT_DIRECTIONAL, // Directional light mostly used for sun
    LIGHT_POINT, // Point light mostly used for torch like items
    LIGHT_SPOT, // Spotlight mostly used for flashlight
};
static int currentLightCount;
struct Light {
    TypeOfLight type; // Lights type
    bool isEnabled; // Checks if light is enabled or not
    float intensity; // Lights intensity
    Vector3 position; // Lights position
    Vector3 direction; // Lights direction
    Color color; // Lights color
    Light(TypeOfLight type, Vector3 position,Vector3 direction, Color color,float intensity) {
        this->type = type; // Sets light type
        isEnabled = true; // Sets light enabled by default
        this->position = position; // Sets light position
        this->direction = direction; // Sets light direction
        this->color = color; // Sets light color
        this->intensity = intensity; // Sets light intensity
        FindShaderLocations(); // Finds shader program related variables
    }
private:
    // Shader locations
    int enabledLoc; // Enabled location in the shader program
    int positionLoc; // Position location in the shader program
    int colorLoc; // Color location in the shader program
    int directionLoc; // Direction location in the shader program
    int typeLoc; // Type location in the shader program
    int intensityLoc; // Intensity location in the shader program

    void FindShaderLocations() {  // This finds shader Locations in shader program at very start. This function shouldn't be entered second time
        Shader &shader = ShaderManager::GetInstance().shader; // Gets shaderManager singleton and then gets its shader as a reference
        enabledLoc = GetShaderLocation(shader, TextFormat("lights[%i].enabled", currentLightCount)); // Gets light enabled location in the shader program
        positionLoc = GetShaderLocation(shader, TextFormat("lights[%i].position", currentLightCount)); // Gets light position location in the shader program
        directionLoc = GetShaderLocation(shader, TextFormat("lights[%i].direction", currentLightCount)); // Gets light direction location in the shader program
        intensityLoc = GetShaderLocation(shader, TextFormat("lights[%i].intensity", currentLightCount)); // Gets light intensity location in the shader program
        colorLoc = GetShaderLocation(shader, TextFormat("lights[%i].color", currentLightCount)); // Gets light color location in the shader program
        typeLoc = GetShaderLocation(shader, TextFormat("lights[%i].type", currentLightCount)); // Gets light type location in the shader program
        currentLightCount++; // Increases current light count by 1
        int currentLightCountLoc = GetShaderLocation(shader, "currentLights"); // Gets current light count location in the shader program
        std::cout << "INFO: Light Position location in shader program: " << positionLoc << "\n";
        std::cout << "INFO: Light Direction location in shader program: " << directionLoc << "\n";
        std::cout << "INFO: Light Color location in shader program: " << colorLoc << "\n";
        std::cout << "INFO: Light Intensity location in shader program: " << intensityLoc << "\n";
        std::cout << "INFO: Light Type location in shader program: " << typeLoc << "\n";
        std::cout << "INFO: Light Enabled location in shader program: " << enabledLoc << "\n";
        std::cout << "INFO: Current Light Count location in shader program: " << currentLightCountLoc << "\n";
        UpdateLightValues(); // First time update
    }

    void UpdateLightValues() {  // Update Shaders programs light values, this function should trigger when you move or change the intensity in light.
        Shader &shader = ShaderManager::GetInstance().shader; // Gets shaderManager singleton and then gets its shader as a reference
        int isEnabledInt = isEnabled ? 1 : 0; // Converts isEnabled to int
        SetShaderValue(shader, enabledLoc, &isEnabledInt, SHADER_UNIFORM_INT); // Sets shader program's light enabled
        SetShaderValue(shader, positionLoc, &position, SHADER_UNIFORM_VEC3); // Sets shader program's light position
        SetShaderValue(shader, directionLoc, &direction, SHADER_UNIFORM_VEC3); // Sets Shader program's light direction
        SetShaderValue(shader, intensityLoc, &intensity, SHADER_UNIFORM_FLOAT); // Sets Shader program's light intensity
        Vector3 normalizedColor = {color.r / 255.0f, color.g / 255.0f, color.b / 255.0f}; // Normalizes color for shader
        SetShaderValue(shader, colorLoc, &normalizedColor, SHADER_UNIFORM_VEC3); // Sets Shader program's light color
        SetShaderValue(shader, typeLoc, &type, SHADER_UNIFORM_INT); // Sets Shader program's light type ( Directional, Point, Spot
        std::cout << "INFO: Light Values Updated\n";
    }
};
#endif
