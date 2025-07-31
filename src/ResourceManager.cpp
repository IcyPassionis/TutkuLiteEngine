#include "ResourceManager.h"
#include "ShaderManager.h"
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
