#pragma once
#include <iostream>
#include <raylib.h>
#include <unordered_map>
#ifndef RESOURCEMANAGER_H
#define RESOURCEMANAGER_H
struct ResourceManager {
    std::unordered_map<std::string, Model> Models ;
    Model barrel;
    ResourceManager() {
        // This Constructor only loads things we know will always be loaded in game
        barrel = LoadModel(RESOURCES_PATH "models/barrel.gltf");
    }
    ~ResourceManager() {
        // Deconstructor SHOULD unload everything
        UnloadModel(barrel);
        for (auto model : Models) {
            UnloadModel(model.second);
        }
    }
};
#endif //RESOURCEMANAGER_H
