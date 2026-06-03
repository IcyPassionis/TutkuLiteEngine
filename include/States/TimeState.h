#pragma once

#ifndef TIMESTATE_H
#define TIMESTATE_H
#include <atomic>
#include <iostream>
#include <thread>
struct TimeState {

    float deltaTime;
    TimeState() {
        isRunning = false;
        fixedTimeStep = 0.016f;
        accumulator = 0.0f;
        std::cout << "MAIN THREAD: " << "Current Fixed Time Step:" << fixedTimeStep << "\n";
    }
    std::thread FixedThread;
    std::atomic<bool> isRunning;
    void FixedUpdateThread();
    void UpdateDeltaTime();
    private:
    float fixedTimeStep;
    float accumulator;
};
#endif //TIMESTATE_H
