#pragma once

#ifndef SCENEMANAGER_H
#define SCENEMANAGER_H
#include <string>
#include "Scene.h"

struct SceneManager // Singleton, scene manager
{
    SceneManager(const SceneManager&) = delete;
    SceneManager& operator=(const SceneManager&) = delete;
    static SceneManager& Get()
    {
        static SceneManager instance;
        return instance;
    }
    SceneManager();
    void LoadSceneByName(const std::string& name); // Load scenes by name of scene
    void LoadSceneByPath(const std::string& path); // Load scenes by file path
    void LoadScene(int id); // Load scenes by id of scene
    Scene ReturnScene(int id); // Return scene object
    private:
    int currentSceneID;
    void CheckScenesInBinary(); // Checks scenes stored in binary files
    std::vector<Scene> loadedScenes; // List of scenes currently loaded
};

#endif