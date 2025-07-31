#pragma once
#include <iostream>
#include <raylib.h>
#include <unordered_map>
#include <vector>
#include "Lighting.h"
#ifndef RESOURCEMANAGER_H
#define RESOURCEMANAGER_H
struct ResourceManager {
    std::unordered_map<std::string, Model> Models;
    Model barrel;
    ResourceManager (const ResourceManager&) = delete;
    ResourceManager& operator=(const ResourceManager&) = delete;
    static ResourceManager& Get() {
        static ResourceManager instance;
        return instance;
    }
    ResourceManager() {
        barrel = LoadModel(RESOURCES_PATH "models/barrel.gltf");
    }
    ~ResourceManager() {  // Deconstructor SHOULD unload everything
        UnloadModels();
    }
    void LoadShadersToModels();

private:
    void UnloadModels();
};
#endif //RESOURCEMANAGER_H
