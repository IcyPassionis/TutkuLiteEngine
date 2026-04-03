#include "ResourceManager.h"
#include "ShaderManager.h"
#include <filesystem>
#include <algorithm>
#include <format>

#include "json.hpp"
#include "SceneManager.h"
ResourceManager::ResourceManager()
{

    if (std::filesystem::exists(RESOURCES_PATH) && !inEngineMode)
    {
        std::cout << "RESOURCE THREAD," << "In engine mode, resources folder founded will load all models for engine";
        LoadAllModels();
        LoadPathsInAssets();
    }
    else if (std::filesystem::exists(RESOURCES_PATH) && inEngineMode)
    {
        LoadPathsInAssets();
    }
    else
    {
        std::cout << "RESOURCE THREAD, WARNING: " << "No resources path founded please create 'resources' folder.";
    }
}

void ResourceManager::LoadShadersToModels()
{
    for (auto& model : currentLoadedModels)
    {
        if (model.second.materials == nullptr) continue;
        for (int i = 0; i < model.second.materialCount; i++)
        {
            model.second.materials[i].shader = ShaderManager::Get().shader;
        }
    }
}
void ResourceManager::UnloadAllModels()
{
    for (auto& model : currentLoadedModels) {
        UnloadModel(model.second);
    }

}
void ResourceManager::LoadAllModels()
{
    std::string modelsPath = RESOURCES_PATH "Models/";
    if (std::filesystem::exists(modelsPath))
    {
        for (const auto& entry : std::filesystem::directory_iterator(modelsPath))
        {
            if (entry.is_regular_file())
            {
                std::cout << entry.path().extension() << std::endl;
                std::string ext = entry.path().extension().string();
                std::ranges::transform(ext, ext.begin(), tolower);
                if (ext == ".obj"
                    || ext == ".fbx"
                    || ext == ".gltf")
                {
                    Model model = LoadModel(entry.path().string().c_str());
                    std::string name = entry.path().filename().replace_extension();
                    std::cout << "Loaded model file name: " << name << std::endl;
                    currentLoadedModels.insert({name, model});
                }
            }
        }
    }
    else
    {
        std::cout << "RESOURCE THREAD, Models file, doesnt exists. Returns" << std::endl;
    }
    LoadShadersToModels();
}
void ResourceManager::LoadAllModelsInScene(const int sceneId)
{
    const std::string modelsPath = RESOURCES_PATH "Models/";
    const std::string assetsPath = RESOURCES_PATH "Assets/";
    Scene scene = SceneManager::Get().ReturnScene(sceneId);
    const auto modelNames= scene.GetModels();

    for (const auto& entry : std::filesystem::directory_iterator(modelsPath))
    {
        if (entry.is_regular_file())
        {
            std::string ext = entry.path().extension().string();
            std::ranges::transform(ext, ext.begin(), tolower);
            if (ext== ".obj"
                   || ext == ".fbx"
                   || ext == ".gltf")
            {
                std::string name = entry.path().filename().replace_extension();
                if (std::ranges::find(modelNames, name) == modelNames.end()){
                    continue;
                }
                Model model = LoadModel(entry.path().string().c_str());
                std::cout << "RESOURCE THREAD, Loaded model file name: " << name << std::endl;
                currentLoadedModels.insert({name, model});
            }
        }
    }
    ResourceManager::LoadShadersToModels();
}
void ResourceManager::LoadPathsInAssets()
{
    std::string assetsPath = RESOURCES_PATH "Assets/";
    std::string modelsPath = RESOURCES_PATH "Models/";
}

void ResourceManager::LoadResourcesForScene(int sceneId) {
    Scene scene = SceneManager::Get().ReturnScene(sceneId);
    const auto& modelNames = scene.GetModels();
    const auto& textureNames = scene.GetTextures();
    const auto& iconNames = scene.GetIcons();
    LoadModelsForScene(modelNames);
    LoadShadersToModels();
    LoadTexturesForScene(textureNames);
    LoadIconsForScene(iconNames);
}

void ResourceManager::UnloadResourcesForScene(const int sceneId) {
    Scene scene = SceneManager::Get().ReturnScene(sceneId);
    const auto& modelNames = scene.GetModels();
    const auto& textureNames = scene.GetTextures();
    const auto& iconNames = scene.GetIcons();
    UnloadModelsForScene(modelNames);
    UnloadTexturesForScene(textureNames);
    UnloadIconsForScene(iconNames);
}
void ResourceManager::SwitchSceneResources(int fromSceneId, int toSceneId) {
    if (fromSceneId == toSceneId) return;
    Scene fromScene = SceneManager::Get().ReturnScene(fromSceneId);
    Scene toScene = SceneManager::Get().ReturnScene(toSceneId);
    const auto& fromModels = fromScene.GetModels();
    const auto& toModels = toScene.GetModels();
    const auto& fromTextures = fromScene.GetTextures();
    const auto& toTextures = toScene.GetTextures();
    const auto& fromIcons = fromScene.GetIcons();
    const auto& toIcons = toScene.GetIcons();
    SwitchSceneModels(fromModels, toModels);
    LoadShadersToModels();
    SwitchSceneTextures(fromTextures, toTextures);
    SwitchSceneIcons(fromIcons, toIcons);
}

void ResourceManager::SwitchSceneModels(const std::vector<std::string>& fromModels, const std::vector<std::string>& toModels)
{
    for (auto iteration = currentLoadedModels.begin(); iteration != currentLoadedModels.end(); ) {
        bool inFrom = std::ranges::find(fromModels, iteration->first) != fromModels.end();
        bool inTo = std::ranges::find(toModels, iteration->first) != toModels.end();
        if (inFrom && !inTo) {
            UnloadModel(iteration->second);
            iteration = currentLoadedModels.erase(iteration);
        } else {
            ++iteration;
        }
    }
    const std::string modelsPath = RESOURCES_PATH "Models/";
    for (const auto& entry : std::filesystem::directory_iterator(modelsPath)) {
        if (entry.is_regular_file()) {
            std::string ext = entry.path().extension().string();
            std::ranges::transform(ext, ext.begin(), tolower);
            if (ext == ".obj" || ext == ".fbx" || ext == ".gltf") {
                std::string name = entry.path().filename().replace_extension();
                bool inTo = std::ranges::find(toModels, name) != toModels.end();
                bool alreadyLoaded = currentLoadedModels.find(name) != currentLoadedModels.end();
                if (inTo && !alreadyLoaded) {
                    Model model = LoadModel(entry.path().string().c_str());
                    currentLoadedModels.insert({name, model});
                }
            }
        }
    }
}
void ResourceManager::SwitchSceneTextures(const std::vector<std::string>& fromTextures, const std::vector<std::string>& toTextures)
{
    for (auto iteration = currentLoaded2DTextures.begin(); iteration != currentLoaded2DTextures.end(); ) {
        bool inFrom = std::ranges::find(fromTextures, iteration->first) != fromTextures.end();
        bool inTo = std::ranges::find(toTextures, iteration->first) != toTextures.end();
        if (inFrom && !inTo) {
            UnloadTexture(iteration->second);
            iteration = currentLoaded2DTextures.erase(iteration);
        } else {
            ++iteration;
        }
    }
    const std::string texturesPath = RESOURCES_PATH "Textures/";
    for (const auto& entry : std::filesystem::directory_iterator(texturesPath)) {
        if (entry.is_regular_file()) {
            std::string ext = entry.path().extension().string();
            std::ranges::transform(ext, ext.begin(), tolower);
            if (ext == ".png" || ext == ".jpg" || ext == ".jpeg") {
                std::string name = entry.path().filename().replace_extension();
                bool inTo = std::ranges::find(toTextures, name) != toTextures.end();
                bool alreadyLoaded = currentLoaded2DTextures.find(name) != currentLoaded2DTextures.end();
                if (inTo && !alreadyLoaded) {
                    Image img = LoadImage(entry.path().string().c_str());
                    Texture2D tex = LoadTextureFromImage(img);
                    UnloadImage(img);
                    currentLoaded2DTextures.insert({name, tex});
                }
            }
        }
    }
}
void ResourceManager::SwitchSceneIcons(const std::vector<std::string>& fromIcons, const std::vector<std::string>& toIcons)
{
    for (auto iteration = currentLoadedIcons.begin(); iteration != currentLoadedIcons.end(); ) {
        bool inFrom = std::ranges::find(fromIcons, iteration->first) != fromIcons.end();
        bool inTo = std::ranges::find(toIcons, iteration->first) != toIcons.end();
        if (inFrom && !inTo) {
            UnloadImage(iteration->second);
            iteration = currentLoadedIcons.erase(iteration);
        } else {
            ++iteration;
        }
    }
    const std::string iconsPath = RESOURCES_PATH "Icons/";
    for (const auto& entry : std::filesystem::directory_iterator(iconsPath)) {
        if (entry.is_regular_file()) {
            std::string ext = entry.path().extension().string();
            std::ranges::transform(ext, ext.begin(), tolower);
            if (ext == ".png" || ext == ".jpg" || ext == ".jpeg") {
                std::string name = entry.path().filename().replace_extension();
                bool inTo = std::ranges::find(toIcons, name) != toIcons.end();
                bool alreadyLoaded = currentLoadedIcons.find(name) != currentLoadedIcons.end();
                if (inTo && !alreadyLoaded) {
                    Image icon = LoadImage(entry.path().string().c_str());
                    currentLoadedIcons.insert({name, icon});
                }
            }
        }
    }
}

void ResourceManager::LoadModelsForScene(const std::vector<std::string>& models)
{
    const std::string modelsPath = RESOURCES_PATH "Models/";
    for (const auto& entry : std::filesystem::directory_iterator(modelsPath)) {
        if (entry.is_regular_file()) {
            std::string ext = entry.path().extension().string();
            std::ranges::transform(ext, ext.begin(), tolower);
            if (ext == ".obj" || ext == ".fbx" || ext == ".gltf") {
                std::string name = entry.path().filename().replace_extension();
                if (std::ranges::find(models, name) != models.end()) {
                    if (currentLoadedModels.find(name) == currentLoadedModels.end()) {
                        Model model = LoadModel(entry.path().string().c_str());
                        currentLoadedModels.insert({name, model});
                    }
                }
            }
        }
    }
}
void ResourceManager::LoadTexturesForScene(const std::vector<std::string>& textures)
{
    const std::string texturesPath = RESOURCES_PATH "Textures/";
    for (const auto& entry : std::filesystem::directory_iterator(texturesPath)) {
        if (entry.is_regular_file()) {
            std::string ext = entry.path().extension().string();
            std::ranges::transform(ext, ext.begin(), tolower);
            if (ext == ".png" || ext == ".jpg" || ext == ".jpeg") {
                std::string name = entry.path().filename().replace_extension();
                if (std::ranges::find(textures, name) != textures.end()) {
                    if (currentLoaded2DTextures.find(name) == currentLoaded2DTextures.end()) {
                        Image img = LoadImage(entry.path().string().c_str());
                        Texture2D tex = LoadTextureFromImage(img);
                        UnloadImage(img);
                        currentLoaded2DTextures.insert({name, tex});
                    }
                }
            }
        }
    }
}
void ResourceManager::LoadIconsForScene(const std::vector<std::string>& icons)
{
    const std::string iconsPath = RESOURCES_PATH "Icons/";
    for (const auto& entry : std::filesystem::directory_iterator(iconsPath)) {
        if (entry.is_regular_file()) {
            std::string ext = entry.path().extension().string();
            std::ranges::transform(ext, ext.begin(), tolower);
            if (ext == ".png" || ext == ".jpg" || ext == ".jpeg") {
                std::string name = entry.path().filename().replace_extension();
                if (std::ranges::find(icons, name) != icons.end()) {
                    if (currentLoadedIcons .find(name) == currentLoadedIcons.end()) {
                        Image icon = LoadImage(entry.path().string().c_str());
                        currentLoadedIcons.insert({name, icon});
                    }
                }
            }
        }
    }
}

void ResourceManager::UnloadModelsForScene(const std::vector<std::string>& models)
{
    for (auto iteration = currentLoadedModels.begin(); iteration != currentLoadedModels.end(); ) {
        if (std::ranges::find(models, iteration->first) != models.end()) {
            UnloadModel(iteration->second);
            iteration = currentLoadedModels.erase(iteration);
        }
        else {
            ++iteration;
        }
    }
}
void ResourceManager::UnloadTexturesForScene(const std::vector<std::string>& textures)
{
    for (auto iteration = currentLoaded2DTextures.begin(); iteration != currentLoaded2DTextures.end(); ) {
        if (std::ranges::find(textures, iteration->first) != textures.end()) {
            UnloadTexture(iteration->second);
            iteration = currentLoaded2DTextures.erase(iteration);
        } else {
            ++iteration;
        }
    }
}
void ResourceManager::UnloadIconsForScene(const std::vector<std::string>& icons)
{
    for (auto iteration = currentLoadedIcons.begin(); iteration != currentLoadedIcons.end(); ) {
        if (std::ranges::find(icons, iteration->first) != icons.end()) {
            UnloadImage(iteration->second);
            iteration = currentLoadedIcons.erase(iteration);
        } else {
            ++iteration;
        }
    }
}

