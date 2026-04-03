#include "SceneManager.h"
#include "Scene.h"
#include "ResourceManager.h"
#include <algorithm>
#include <string>

Scene SceneManager::ReturnScene(const int id) { return loadedScenes.at(id); }

SceneManager::SceneManager() { CheckScenesInBinary(); }
void SceneManager::CheckScenesInBinary() {}

Scene SceneManager::LoadScene(int id)
{
    if (loadedScenes.empty())
    {
        auto scene = Scene("empty");
        return scene;
    }
    return loadedScenes[id];
}

int SceneManager::GetSceneIDByName(const std::string &name) {
    auto it = std::ranges::find_if(
        loadedScenes, [&name](const Scene &scene) { return scene.name == name; });
    if (it != loadedScenes.end()) {
        return static_cast<int>(it - loadedScenes.begin());
    }
    return -1;
}

void SceneManager::SwitchScene(int sceneId) {
    ResourceManager::Get().SwitchSceneResources(currentSceneID, sceneId);
    currentSceneID = sceneId;
}

void SceneManager::LoadSceneByName(const std::string& name) {
    int id = GetSceneIDByName(name);
    if (id >= 0) {
        SwitchScene(id);
    }
}

void SceneManager::LoadSceneByPath(const std::string& path) {
}
