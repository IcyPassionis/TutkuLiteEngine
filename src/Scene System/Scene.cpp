#include "Scene.h"
#include "ResourceManager.h"
Scene::Scene(const std::string& name){
    this->name = name;
}
Scene::~Scene(){

}
void Scene::Load(){

}
void Scene::AddModel(const std::string& name){
    loadedModels.push_back(name);
}
void Scene::AddIcon(const std::string& name){
    loadedIcons.push_back(name);
}
void Scene::AddTexture(const std::string& name){
    loaded2DTextures.push_back(name);
}
void Scene::AddModels(std::vector<std::string>& names){
    loadedModels.insert(loadedModels.end(), names.begin(), names.end());
}
void Scene::AddTextures(std::vector<std::string>& names){
    loaded2DTextures.insert(loaded2DTextures.end(), names.begin(), names.end());
}
void Scene::AddIcons(std::vector<std::string>& names){
    loadedIcons.insert(loadedIcons.end(), names.begin(), names.end());
}
void Scene::DrawScene(){
    for (const GameObject& gameObject : gameObjects) gameObject.Draw();
}
void Scene::AddGameObject(const GameObject& gameObject) {
    gameObjects.push_back(gameObject);
}
void Scene::FixedUpdate() {
    std::cout << "Meee" << std::endl;
}

const std::vector<std::string>& Scene::GetAllModels(){
   return loadedModels;
}
const std::vector<std::string>& Scene::GetAllTextures(){
    return loaded2DTextures;
}
const std::vector<std::string>& Scene::GetAllIcons(){
    return loadedIcons;
}
