#pragma once
#ifndef RENDERER3D_H
#define RENDERER3D_H

#include <r3d/r3d.h>

struct Renderer3D
{
    Renderer3D(const Renderer3D&) = delete;
    Renderer3D& operator=(const Renderer3D&) = delete;

    static Renderer3D& Get();

    bool Init(int width, int height);
    void Resize(int width, int height);
    void Begin(const Camera3D& camera);
    void End();
    void Close();
    bool IsInitialized() const;

private:
    Renderer3D() = default;

    bool isInitialized = false;
    int resolutionWidth = 0;
    int resolutionHeight = 0;
};

#endif
