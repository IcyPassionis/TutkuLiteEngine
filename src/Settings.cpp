#include "Settings.hpp"
#include <filesystem>
#include <fstream>
#include <iostream>

using string = std::string;
Settings::Settings()
{
    string settings_file = GAME_PATH "settings.json";
    std::cout << "INFO: Settings file path: " << settings_file << "\n";
    if (!std::filesystem::exists(settings_file))
        SetDefaultSettings();
    else
        LoadSettings();
}

void Settings::SetDefaultSettings()
{
    std::cout << "WARNING: " << "Settings cant be loaded or doesn't exists yet. Default values will be used... \n";
    fileVersion = 1;
    currentMonitor = 0;
    musicVolume = 100;
    audioVolume = 100;
    isVsyncEnabled = true;
    windowWidth = 1920;
    windowHeight = 1080;
    isFpsLocked = false;
    fps = 60;
}

void Settings::LoadSettings()
{
    std::cout << "INFO: " << "Settings currently loading... \n";
    std::ifstream file(GAME_PATH "settings.json");
    try
    {
        json data = json::parse(file);
        FromJson(data);
    }
    catch (const std::exception& e)
    {
        std::cout << e.what() << "\n";
        SetDefaultSettings(); // If any exception happens settings will load default values
    }
}

void Settings::FromJson(const json& json)
{
    json.at("file version").get_to(fileVersion);
    json.at("width").get_to(windowWidth);
    json.at("height").get_to(windowHeight);
    json.at("audio").get_to(audioVolume);
    json.at("music").get_to(musicVolume);
    json.at("vsync").get_to(isVsyncEnabled);
    json.at("current monitor").get_to(currentMonitor);
    json.at("fps locked").get_to(isFpsLocked);
    json.at("fps").get_to(fps);

    std::cout << "Current fps: " << fps << "\n";
}

void Settings::ToJson(json& j)
{
    j = json{
    {"width", windowWidth},
    };
}