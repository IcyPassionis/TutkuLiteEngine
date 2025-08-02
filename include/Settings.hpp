#pragma once

#ifndef SETTINGS_HPP
#define SETTINGS_HPP
#include <json.hpp>
using json = nlohmann::json;
struct Settings {
    short fileVersion;
    short musicVolume;
    short audioVolume;
    int currentMonitor;
    int windowWidth;
    int windowHeight;
    bool isVsyncEnabled;
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
    void SaveSettings(); // Save settings.
private:
    void FromJson(const json& json); // Load json file to settings
    void ToJson(json& j); // Save settings to a json file
    void SetDefaultSettings(); // Sets settings default values, if there is not any settings file available.
    void LoadSettings(); // Loads settings from file
};
extern Settings settings;

#endif
