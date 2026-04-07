#pragma once
#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H
#include <string>
#include <vector>

#include "raylib.h"


class GameObject
{
    std::string modelName;
    void CheckIsActive();
    void UpdateModelTransform();
public:
    Transform transform;
    std::string name;
    std::vector<std::string> tags;
    bool isActive;
    void SetModel(const std::string& modelName);
    const Model& GetModel() const;
    void Update(); // Update Game Object
};
#endif
