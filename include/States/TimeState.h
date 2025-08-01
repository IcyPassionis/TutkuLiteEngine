#pragma once

#ifndef TIMESTATE_H
#define TIMESTATE_H
#include <atomic>
#include <iostream>
#include <thread>
struct TimeState {

    float deltaTime;
    TimeState() {
        isDeltaTime = false;
        fixedTimeStep = 0.016f;
        accumulator = 0.0f;
        std::cout << "Current Fixed Time Step:" << fixedTimeStep << "\n";
    }
    std::thread FixedThread;
    bool FixedUpdate();
    void FixedUpdateThread();
    void UpdateDeltaTime();
    private:
    std::atomic<bool> isDeltaTime;
    float fixedTimeStep;
    float accumulator;
};
#endif //TIMESTATE_H
