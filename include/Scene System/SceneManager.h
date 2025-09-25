#pragma once

#ifndef SCENEMANAGER_H
#define SCENEMANAGER_H
#include <string>
#include "Scene.h"

struct SceneManager // Singleton, scene manager
{
public:
    SceneManager(const SceneManager&) = delete;
    SceneManager& operator=(const SceneManager&) = delete;
    static SceneManager& Get()
    {
        static SceneManager instance;
        return instance;
    }
    void LoadSceneByName(const std::string& name);
    void LoadSceneByPath(const std::string& path);
    void LoadScene(int id);
    private:
    std::vector<Scene> loadedScenes;
};

#endif