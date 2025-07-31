#pragma once

#ifndef STATES_H
#define STATES_H
#include <string>
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
struct States {
    int width; // Screens width
    int height; // Screens height
    std::string title; // Windows title text
    States();
private:
    void Load(); // Loads window State, by getting default values or from settings file
};
#endif //STATES_H
