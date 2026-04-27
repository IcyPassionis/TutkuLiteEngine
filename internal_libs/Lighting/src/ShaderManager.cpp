#include "ShaderManager.h"
#include "Lighting.h"
#include "../../../include/DrawGame.h"
#include <rlgl.h>
#include <raymath.h>
#include "external/glad.h"

static RenderTexture2D LoadShadowmapRenderTexture(int width, int height) {
    RenderTexture2D target = { 0 };
    target.id = rlLoadFramebuffer();
    target.texture.width = width;
    target.texture.height = height;

    rlEnableFramebuffer(target.id);

    // No color attachment — required for FBO completeness on OpenGL 3.3
    glDrawBuffer(GL_NONE);
    glReadBuffer(GL_NONE);

    target.depth.id = rlLoadTextureDepth(width, height, false);
    target.depth.width = width;
    target.depth.height = height;
    target.depth.format = 19; // DEPTH_COMPONENT_24BIT
    target.depth.mipmaps = 1;
    rlFramebufferAttach(target.id, target.depth.id, RL_ATTACHMENT_DEPTH, RL_ATTACHMENT_TEXTURE2D, 0);
    rlFramebufferComplete(target.id);
    rlDisableFramebuffer();

    return target;
}

static void UnloadShadowmapRenderTexture(RenderTexture2D target) {
    if (target.id > 0) {
        rlUnloadFramebuffer(target.id);
        if (target.depth.id > 0) rlUnloadTexture(target.depth.id);
    }
}

void ShaderManager::LoadShaders() {
    shader = LoadShader(SHADERS_PATH "glsl100/lighting.vert", SHADERS_PATH "glsl100/lighting.frag");
    ambientLoc = GetShaderLocation(shader, "ambientColor");
    ambientStrengthLoc = GetShaderLocation(shader, "ambientStrength");
    materialColorLoc = GetShaderLocation(shader, "materialColor");
    shininessLoc = GetShaderLocation(shader, "shininess");
    float a = 0.25f;
    SetShaderValue(shader, ambientStrengthLoc, &a, SHADER_UNIFORM_FLOAT);
    float ambientColor[3] = {0.3f, 0.4f, 0.6f};
    SetShaderValue(shader, ambientLoc, ambientColor, SHADER_UNIFORM_VEC3);
    shader.locs[SHADER_LOC_VECTOR_VIEW] = GetShaderLocation(shader, "viewPos");

    lightSpaceMatrixLoc = GetShaderLocation(shader, "lightSpaceMatrix");
    shadowMapLoc = GetShaderLocation(shader, "shadowMap");

    int shadowSlot = 1;
    SetShaderValue(shader, shadowMapLoc, &shadowSlot, SHADER_UNIFORM_INT);

    LoadDepthShader();
    shadowMap = LoadShadowmapRenderTexture(2048, 2048);

    isInitialized = true;
}

void ShaderManager::LoadDepthShader() {
    depthShader = LoadShader(SHADERS_PATH "glsl100/shadow.vert", SHADERS_PATH "glsl100/shadow.frag");
}

void ShaderManager::UnloadShaders() {
    UnloadShader(shader);
    UnloadShader(depthShader);
    UnloadShadowmapRenderTexture(shadowMap);
    isInitialized = false;
}

void ShaderManager::ReloadLights() {
    if (lights.size() > 0) {
        for (int i = 0; i < lights.size(); i++) {
            lights[i].ReloadLight();
        }
    }
}

void ShaderManager::UpdateShadowMap() {
    if (lights.empty()) return;

    Light* dirLight = nullptr;
    for (auto& light : lights) {
        if (light.type == LIGHT_DIRECTIONAL && light.isEnabled) {
            dirLight = &light;
            break;
        }
    }
    if (!dirLight) return;

    Vector3 lightDir = Vector3Normalize(dirLight->direction);
    Vector3 up = {0.0f, 1.0f, 0.0f};
    if (fabsf(Vector3DotProduct(lightDir, up)) > 0.99f)
        up = {1.0f, 0.0f, 0.0f};

    Vector3 lightPos = Vector3Scale(lightDir, -50.0f);

    Camera3D lightCam = {};
    lightCam.position = lightPos;
    lightCam.target = Vector3Add(lightPos, lightDir);
    lightCam.up = up;
    lightCam.fovy = 120.0f;
    lightCam.projection = CAMERA_ORTHOGRAPHIC;

    BeginTextureMode(shadowMap);
    ClearBackground(WHITE);
    BeginMode3D(lightCam);

    Matrix lightView = rlGetMatrixModelview();
    Matrix lightProj = rlGetMatrixProjection();

    BeginShaderMode(depthShader);
    DrawScene();
    EndShaderMode();

    EndMode3D();
    EndTextureMode();

    Matrix lightSpaceMat = MatrixMultiply(lightView, lightProj);
    SetShaderValueMatrix(shader, lightSpaceMatrixLoc, lightSpaceMat);
}
