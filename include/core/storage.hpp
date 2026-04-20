#pragma once

#include <nlohmann/json.hpp>
#include <string>
#include <filesystem>

class Storage {
public:
    static bool saveJson(const std::string& path, const nlohmann::json& j);
    static nlohmann::json loadJson(const std::string& path);
    static bool ensureDirectory(const std::string& path);
};
