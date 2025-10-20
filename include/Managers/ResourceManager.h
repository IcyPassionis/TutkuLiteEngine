#pragma once
#include <iostream>
#include <raylib.h>
#include <unordered_map>
#include "Lighting.h"
#ifndef RESOURCEMANAGER_H
#define RESOURCEMANAGER_H
struct ResourceManager {
    std::unordered_map<std::string, Model> currentLoadedModels;
    std::unordered_map<std::string, Material> currentLoadedMaterials;
    std::unordered_map<std::string, Texture2D> currentLoaded2DTextures;
    std::unordered_map<std::string, Image> currentLoadedIcons;
    std::unordered_map<std::string, std::string> loadedModelsPath;
    bool inBuildMode = false;
    Model barrel;
    ResourceManager (const ResourceManager&) = delete;
    ResourceManager& operator=(const ResourceManager&) = delete;
    static ResourceManager& Get() {
        static ResourceManager instance;
        return instance;
    }
    ResourceManager();
    ~ResourceManager() {  // Deconstructor SHOULD unload everything
        UnloadModels();
    }
    void LoadShadersToModels();
    void LoadModelsInScene(int id);
private:
    void LoadPathsInAssets();
    void LoadAllModels();
    void UnloadAllModels();
};
#endif //RESOURCEMANAGER_H
