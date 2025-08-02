#include "Settings.hpp"
#include <filesystem>
Settings::Settings()
{
    std::string settings_file = GAME_PATH "settings.json";
    if (!std::filesystem::exists(settings_file))
        SetDefaultSettings();
}

void Settings::SetDefaultSettings()
{
    fileVersion = 1;
    currentMonitor = 0;
    musicVolume = 100;
    audioVolume = 100;
    windowWidth = 1920;
    windowHeight = 1080;
    isFpsLocked = true;
    fps = 60;
}
