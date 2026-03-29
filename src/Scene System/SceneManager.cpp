#include "SceneManager.h"
#include "Scene.h"
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
  auto id = std::ranges::find_if(
      loadedScenes, [&name](const Scene &scene) { return scene.name == name; });
  return 0;
}
