#pragma once
#include <iostream>
#include <raylib.h>
#include <unordered_map>
#include <vector>

#include "Lighting.h"
#ifndef RESOURCEMANAGER_H
#define RESOURCEMANAGER_H
struct ResourceManager {
    std::unordered_map<std::string, Model> currentLoadedModels;
    std::unordered_map<std::string, Material> currentLoadedMaterials;
    std::unordered_map<std::string, Texture2D> currentLoaded2DTextures;
    std::unordered_map<std::string, Image> currentLoadedIcons;
    std::unordered_map<std::string, std::string> loadedModelsPath;
    bool inEngineMode = false;
    ResourceManager (const ResourceManager&) = delete;
    ResourceManager& operator=(const ResourceManager&) = delete;
    static ResourceManager& Get() {
        static ResourceManager instance;
        return instance;
    }
    ResourceManager();
    ~ResourceManager() {  // Deconstructor SHOULD unload everything
        UnloadAllModels();
    }
    void LoadShadersToModels();
    void LoadAllModelsInScene(const int sceneId);
    void SwitchSceneResources(int fromSceneId, int toSceneId);
    void LoadResourcesForScene(int sceneId);
    void UnloadResourcesForScene(const int sceneId);
private:
    void LoadPathsInAssets();
    void LoadAllModels();
    void UnloadModelsForScene(const std::vector<std::string>& models);
    void UnloadTexturesForScene(const std::vector<std::string>& textures);
    void UnloadIconsForScene(const std::vector<std::string>& icons);
    void LoadModelsForScene(const std::vector<std::string>& models);
    void LoadTexturesForScene(const std::vector<std::string>& textures);
    void LoadIconsForScene(const std::vector<std::string>& icons);
    void SwitchSceneModels(const std::vector<std::string>& fromModels, const std::vector<std::string>& toModels);
    void SwitchSceneTextures(const std::vector<std::string>& fromTextures, const std::vector<std::string>& toTextures);
    void SwitchSceneIcons(const std::vector<std::string>& fromIcons, const std::vector<std::string>& toIcons);
    void UnloadAllModels();
};
#endif //RESOURCEMANAGER_H
