#pragma once

#ifndef SETTINGS_HPP
#define SETTINGS_HPP
struct Settings {
    short fileVersion;
    short musicVolume;
    short audioVolume;
    int currentMonitor;
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
    Settings();
private:
    void SetDefaultSettings(); // Sets settings default values, if there is not any settings file available.
};
extern Settings settings;

#endif
