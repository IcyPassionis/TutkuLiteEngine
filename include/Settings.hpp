#pragma once

#ifndef SETTINGS_HPP
#define SETTINGS_HPP
struct Settings {
    unsigned short fileVersion;
    unsigned short musicVolume;
    unsigned short audioVolume;
    unsigned int windowWidth;
    unsigned int windowHeight;
    bool isFpsLocked;
    unsigned int fps;
    Settings() {
        windowWidth = 0;
        windowHeight = 0;
        isFpsLocked = true;
        fps = 60;
    }
};
extern Settings settings;

#endif
