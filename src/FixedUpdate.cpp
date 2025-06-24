#include "raylib.h"
#include "TimeState.h"
#include <atomic>
#include <chrono>
#include <thread>

// A physic based update use when calculating anything physics related
bool TimeState::FixedUpdate() {
    return isDeltaTime.load();
}

void TimeState::FixedUpdateThread() {
    while (!WindowShouldClose()) {
        std::this_thread::sleep_for(std::chrono::milliseconds(16));
        accumulator += deltaTime;
        while (accumulator >= fixedTimeStep) {
            accumulator -= fixedTimeStep;
            isDeltaTime.store(true);
        }
        if (accumulator < fixedTimeStep) {
            isDeltaTime.store(false);
        }
    }
}
void TimeState::UpdateDeltaTime() {
    deltaTime = GetFrameTime();
}

