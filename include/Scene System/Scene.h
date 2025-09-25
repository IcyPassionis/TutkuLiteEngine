#pragma once

#ifndef SCENE_H
#define SCENE_H
#include <string>
#include <vector>


class Scene // Scene object, serializable
{
    public:
    std::string name; // Scene's name in scenes list, should be unique

    Scene(std::string name) {
        this->name = name;
    }
private:
    std::vector<std::string> loadedModels; // Loaded models name in this scene
    std::vector<std::string> loaded2DTextures; // Loaded textures in this scene
    std::vector<std::string> loadedIcons; // Loaded UI icons in this scene
};
#endif