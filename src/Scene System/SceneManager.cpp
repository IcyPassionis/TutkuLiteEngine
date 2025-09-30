#include "SceneManager.h"
#include "Scene.h"

Scene SceneManager::ReturnScene(const int id)
{
    return loadedScenes.at(id);
}

SceneManager::SceneManager()
{
    CheckScenesInBinary();
}
void SceneManager::CheckScenesInBinary()
{

}
