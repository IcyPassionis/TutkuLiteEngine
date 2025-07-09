#pragma once

#ifndef LIGHTING_H
#define LIGHTING_H
#define MAX_LIGHTS 20
#include <raylib.h>
struct ShaderManager;

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
    Camera3D camera;
    Light(TypeOfLight type, Vector3 position,Vector3 direction, Color color,float intensity) {
        this->type = type; // Sets light type
        isEnabled = true; // Sets light enabled by default
        this->position = position; // Sets light position
        this->direction = direction; // Sets light direction
        this->color = color; // Sets light color
        this->intensity = intensity; // Sets light intensity
        FindShaderLocations(); // Finds shader program related variables
    }
    void ReloadLight(); // Reload Shaders of light
private:
    // Shader locations
    int enabledLoc; // Enabled location in the shader program
    int positionLoc; // Position location in the shader program
    int colorLoc; // Color location in the shader program
    int directionLoc; // Direction location in the shader program
    int typeLoc; // Type location in the shader program
    int intensityLoc; // Intensity location in the shader program

    void FindShaderLocations(); // This finds shader Locations in shader program at very start. This function shouldn't be entered second time

    void UpdateLightValues(); // Update Shaders programs light values, this function should trigger when you move or change the intensity in light.

};
#endif
