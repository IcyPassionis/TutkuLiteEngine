#include "ResourceManager.h"
#include "ShaderManager.h"
#include <filesystem>
#include <algorithm>
#include "SceneManager.h"
ResourceManager::ResourceManager()
{
    if (std::filesystem::exists(RESOURCES_PATH))
    {
        LoadAllModels();
        LoadPathsInAssets();
    }
    else
    {
        std::cout << "RESOURCE THREAD, WARNING: " << "No resources path founded please create 'resources' folder.";
    }
}

void ResourceManager::LoadShadersToModels()
{
    for (auto& model : loadedModels)
    {
        for (int i = 0; i < model.second.materialCount; i++)
        {
            model.second.materials[i].shader = ShaderManager::Get().shader;
        }
    }
    for (int i =0; i < barrel.materialCount;i++) {
        barrel.materials[i].shader = ShaderManager::Get().shader;
    }
}
void ResourceManager::UnloadModels()
{
    for (auto& model : loadedModels) {
        UnloadModel(model.second);
    }
    UnloadModel(barrel);
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
                std::transform(ext.begin(), ext.end(), ext.begin(), tolower);
                if (entry.path().extension() == ".obj"
                    || entry.path().extension() == ".fbx"
                    || entry.path().extension() == ".gltf")
                {
                    Model model = LoadModel(entry.path().string().c_str());
                    std::string name = entry.path().filename().replace_extension();
                    std::cout << "Loaded model file name: " << name << std::endl;
                    loadedModels.insert({name, model});
                }
            }
        }
    }
    else
    {
        std::cout << "Models file, doesnt exists. Returns" << std::endl;
        barrel = LoadModel(RESOURCES_PATH "models/barrel.gltf");
    }
}
void ResourceManager::LoadModelsInScene(int id)
{
    Scene scene = SceneManager::Get().ReturnScene(id);
}
void ResourceManager::LoadPathsInAssets()
{
    std::string assetsPath = RESOURCES_PATH "Assets/";
    std::string modelsPath = RESOURCES_PATH "Models/";

}
