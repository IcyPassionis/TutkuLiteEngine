#include "GameObject.h"

#include <iostream>
#include <raymath.h>

#include "ResourceManager.h"

GameObject::GameObject()
    : transform{{0, 0, 0}, QuaternionIdentity(), {1, 1, 1}} {}

void GameObject::SetModel(const std::string& modelName) {
  auto& resources = ResourceManager::Get();
  if (resources.currentLoadedModels.contains(modelName)) {
    this->modelName = modelName;
  } else {
    std::cout << "MAIN THREAD on GameObject::SetModel() " << modelName
              << " NOT FOUND ON RESOURCE MANAGER" << std::endl;
  }
}

const R3D_Model* GameObject::GetModel() const {
  if (modelName.empty()) return nullptr;
  const auto& models = ResourceManager::Get().currentLoadedModels;
  const auto found = models.find(modelName);
  return found == models.end() ? nullptr : &found->second;
}

void GameObject::Draw() const {
  if (!isActive) return;
  const R3D_Model* model = GetModel();
  if (model == nullptr) return;
  R3D_DrawModelEx(*model, transform.translation, transform.rotation,
                  transform.scale);
}
