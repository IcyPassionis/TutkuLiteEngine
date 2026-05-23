#pragma once

#ifndef SCENE_H
#define SCENE_H
#include <string>
#include <vector>

#include "raylib.h"
#include "Object System/GameObject.h"

class Scene // Scene object, serializable
{
    public:
    std::string name = "default"; // Scene's name in scenes list, should be unique
    Scene(const std::string& name); // Constructor
    ~Scene(); // Deconstructor
    virtual void Load(); // Load current scene with models,textures,icons.
    void DrawScene(); // Draw current scene to game
    //virtual void Update(); // Frame-Rate Based update.
    virtual void FixedUpdate(); // Fixed timestep update for physics/deterministic logic
    void AddModel(const std::string& name);
    void AddTexture(const std::string& name);
    void AddIcon(const std::string& name);
    void AddModels(std::vector<std::string>& names); // Load multiple models(name) by a vector
    void AddTextures(std::vector<std::string>& names); // Load multiple textures(name) by a vector
    void AddIcons(std::vector<std::string>& names); // Load multiple icons(name) by a vector
    const std::vector<std::string>& GetAllModels(); // Gets every model name into an array.
    const std::vector<std::string>& GetAllTextures(); // Gets every texture name
    const std::vector<std::string>& GetAllIcons(); // Gets every icon name

private:
    std::vector<GameObject> gameObjects;
    std::vector<std::string> loadedModels; // Loaded models name in this scene
    std::vector<std::string> loaded2DTextures; // Loaded textures in this scene
    std::vector<std::string> loadedIcons; // Loaded UI icons in this scene
};
#endif