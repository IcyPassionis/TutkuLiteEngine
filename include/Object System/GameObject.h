#pragma once
#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H
#include <string>
#include <vector>

#include <raylib.h>
#include <r3d/r3d.h>


class GameObject
{
    std::string modelName;
    std::string id;
    void CheckIsActive();
    void UpdateModelTransform();
public:
    GameObject();
    void Draw() const; // Draw GameObject
    Transform transform;
    std::string name;
    std::vector<std::string> tags;
    bool isActive = true;
    void SetModel(const std::string& modelName);
    const R3D_Model* GetModel() const;
    void Update(); // Update Game Object
};
#endif
