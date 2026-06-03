#include "TimeState.h"
#include "SceneManager.h"
#include <atomic>
#include <chrono>
#include <thread>

void TimeState::FixedUpdateThread() {
    auto lastTime = std::chrono::steady_clock::now();

    while (isRunning.load()) {
        auto currentTime = std::chrono::steady_clock::now();
        std::chrono::duration<float> elapsed = currentTime - lastTime;
        lastTime = currentTime;

        accumulator += elapsed.count();

        while (accumulator >= fixedTimeStep) {
            accumulator -= fixedTimeStep;
            auto& sm = SceneManager::Get();
            std::lock_guard<std::mutex> lock(sm.sceneMutex);
            Scene* currentScene = sm.GetCurrentScene();
            if (currentScene != nullptr) {
                currentScene->FixedUpdate();
            }
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }
}

void TimeState::UpdateDeltaTime() {
    deltaTime = GetFrameTime();
}
