#include "Rendering/Renderer3D.h"

Renderer3D& Renderer3D::Get()
{
    static Renderer3D renderer;
    return renderer;
}

bool Renderer3D::Init(const int width, const int height)
{
    if (isInitialized)
    {
        return true;
    }

    if (!IsWindowReady() || width <= 0 || height <= 0)
    {
        return false;
    }

    const int pixelWidth = GetRenderWidth();
    const int pixelHeight = GetRenderHeight();
    const int renderWidth = pixelWidth > 0 ? pixelWidth : width;
    const int renderHeight = pixelHeight > 0 ? pixelHeight : height;

    isInitialized = R3D_Init(renderWidth, renderHeight);
    if (isInitialized)
    {
        resolutionWidth = renderWidth;
        resolutionHeight = renderHeight;
    }

    return isInitialized;
}

void Renderer3D::Resize(const int width, const int height)
{
    if (!isInitialized || width <= 0 || height <= 0)
    {
        return;
    }

    const int pixelWidth = GetRenderWidth();
    const int pixelHeight = GetRenderHeight();
    const int renderWidth = pixelWidth > 0 ? pixelWidth : width;
    const int renderHeight = pixelHeight > 0 ? pixelHeight : height;
    if (renderWidth == resolutionWidth && renderHeight == resolutionHeight)
    {
        return;
    }

    R3D_SetResolution(renderWidth, renderHeight);
    resolutionWidth = renderWidth;
    resolutionHeight = renderHeight;
}

void Renderer3D::Begin(const Camera3D& camera)
{
    if (isInitialized)
    {
        R3D_Begin(camera);
    }
}

void Renderer3D::End()
{
    if (isInitialized)
    {
        R3D_End();
    }
}

void Renderer3D::Close()
{
    if (!isInitialized)
    {
        return;
    }

    R3D_Close();
    isInitialized = false;
    resolutionWidth = 0;
    resolutionHeight = 0;
}

bool Renderer3D::IsInitialized() const
{
    return isInitialized;
}
