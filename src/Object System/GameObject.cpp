#include "GameObject.h"

#include <string>

#include "ResourceManager.h"
#include "SceneManager.h"

void GameObject::SetModel(const std::string& modelName)
{
    auto& resourceManager = ResourceManager::Get();
    if (resourceManager.currentLoadedModels.contains(modelName))
    {
        this->modelName = modelName;
    }
    else
    {
        std::cout << "MAIN THREAD " << "on GameObject::SetModel() " << modelName << " NOT FOUND ON RESOURCE MANAGER" << std::endl;
    }
}

const Model& GameObject::GetModel() const
{
    auto& resourceManager = ResourceManager::Get();
    if (modelName.empty())
    {
        std::cout << "MAIN THREAD " << "on GameObject::GetModel() No model name has given !" << std::endl;
        static Model nullModel = {};
        return nullModel;
    }
    if (resourceManager.currentLoadedModels.contains(modelName))
    {
        return resourceManager.currentLoadedModels.at(modelName);
    }
    else
    {
        std::cout << "MAIN THREAD " << "on GameObject::GetModel() Model Name: " << modelName << " NOT FOUND ON RESOURCE MANAGER !" << std::endl;
        static Model nullModel = {};
        return nullModel;
    }
}


