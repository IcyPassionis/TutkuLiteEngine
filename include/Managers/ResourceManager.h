#pragma once
#include <iostream>
#include <raylib.h>
#include <unordered_map>
#include "Lighting.h"
#ifndef RESOURCEMANAGER_H
#define RESOURCEMANAGER_H
struct ResourceManager {
    std::unordered_map<std::string, Model> loadedModels;
    Model barrel;
    ResourceManager (const ResourceManager&) = delete;
    ResourceManager& operator=(const ResourceManager&) = delete;
    static ResourceManager& Get() {
        static ResourceManager instance;
        return instance;
    }
    ResourceManager() {
        LoadAllModels();
    }
    ~ResourceManager() {  // Deconstructor SHOULD unload everything
        UnloadModels();
    }
    void LoadShadersToModels();
    void LoadModelsInScene(int id);
private:
    void LoadPathsInAssets();
    void LoadAllModels();
    void UnloadModels();
};
#endif //RESOURCEMANAGER_H
