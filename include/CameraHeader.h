
#ifndef CAMERAHEADER_H
#define CAMERAHEADER_H
#include <raylib.h>
#include <iostream>
struct CameraManager {
    Camera3D camera;
    CameraManager(const CameraManager&) = delete;
    CameraManager& operator=(const CameraManager&) = delete;
    static CameraManager& Get() {
        static CameraManager instance;
        return instance;
    }
    CameraManager() {
        camera.position = Vector3(10, 20, 5);
        camera.target = Vector3(0, 0, 0);
        camera.up = Vector3(0, 1, 0);
        camera.fovy = 90;
        camera.projection = CAMERA_PERSPECTIVE;
        std::cout << "MAIN THREAD: " << "Camera has been Initialized\n";
    }
};
#endif
