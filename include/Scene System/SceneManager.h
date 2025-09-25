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
    void LoadSceneByName(const std::string& name); // Load scenes by name of scene
    void LoadSceneByPath(const std::string& path); // Load scenes by file path
    void LoadScene(int id); // Load scenes by id of scene
    private:
    std::vector<Scene> loadedScenes; // List of scenes currently loaded
};

#endif