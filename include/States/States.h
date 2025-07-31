#pragma once

#ifndef STATES_H
#define STATES_H
#include <string>
#include <Settings.hpp>
// ScreenState enum this contains every screen player can enter
enum ScreenState {
    MAIN_MENU, // Main menu, default screen
    GAME, // Gameplay screen
    OPTIONS, // Options
};
// GameState controls game-related states
struct GameState {
    ScreenState screenState; // This saves currently which state is player
    bool isScreenOnTransition; // If screen is on transition an animation will play
    bool isFinished; // Is the game finished
    GameState() {
        screenState = MAIN_MENU;
        isScreenOnTransition = false;
        isFinished = false;
    }
};
// This struct controls window
struct WindowState {
    int width; // Screens width
    int height; // Screens height
    std::string title; // Windows title text
    WindowState(Settings &settings) {
        // Default Resolution, if a settings file doesn't yet create
        if (settings.windowHeight == 0 || settings.windowWidth == 0) {
            width = GetScreenWidth();
            height = GetScreenHeight();
        }
        else {
            width = settings.windowWidth;
            height = settings.windowHeight;
        }
        title = "Island Project"; // Default Name for my game
    }
};
#endif //STATES_H
