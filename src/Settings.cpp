#include "Settings.hpp"
#include <filesystem>
Settings::Settings()
{
    std::string settings_file = GAME_PATH "settings.json";
    if (!std::filesystem::exists(settings_file))
        SetDefaultSettings();
}

