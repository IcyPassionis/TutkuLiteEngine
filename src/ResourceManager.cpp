#include "ResourceManager.h"
#include "ShaderManager.h"
#include <filesystem>
#include <algorithm>
void ResourceManager::LoadShadersToModels()
{
    for (auto& model : Models)
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
    for (auto& model : Models) {
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
                    Models.insert({name, model});
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