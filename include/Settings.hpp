#pragma once

#ifndef SETTINGS_HPP
#define SETTINGS_HPP
struct Settings {
    short fileVersion;
    short musicVolume;
    short audioVolume;
    int windowWidth;
    int windowHeight;
    bool isFpsLocked;
    int fps;
    Settings(const Settings&) = delete;
    Settings& operator=(const Settings&) = delete;
    static Settings& Get()
    {
        static Settings instance;
        return instance;
    }
    Settings() {
        SetDefaultSettings();
    }
    void SetDefaultSettings()
    {
        fileVersion = 1;
        musicVolume = 100;
        audioVolume = 100;
        windowWidth = 1920;
        windowHeight = 1080;
        isFpsLocked = true;
        fps = 60;
    }
};
extern Settings settings;

#endif
