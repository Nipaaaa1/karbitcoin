#include "core/storage.hpp"
#include <fstream>
#include <iostream>

namespace fs = std::filesystem;

bool Storage::saveJson(const std::string& path, const nlohmann::json& j) {
    try {
        if (!ensureDirectory(path)) {
            return false;
        }
        std::ofstream file(path);
        if (file.is_open()) {
            file << j.dump(4);
            return true;
        }
    } catch (const std::exception& e) {
        std::cerr << "Error saving JSON: " << e.what() << std::endl;
    }
    return false;
}

nlohmann::json Storage::loadJson(const std::string& path) {
    try {
        if (fs::exists(path)) {
            std::ifstream file(path);
            if (file.is_open()) {
                nlohmann::json j;
                file >> j;
                return j;
            }
        }
    } catch (const std::exception& e) {
        std::cerr << "Error loading JSON: " << e.what() << std::endl;
    }
    return nlohmann::json(); // Returns null json
}

bool Storage::ensureDirectory(const std::string& path) {
    try {
        fs::path p(path);
        if (p.has_parent_path()) {
            fs::path parent = p.parent_path();
            if (!fs::exists(parent)) {
                return fs::create_directories(parent);
            }
        }
        return true;
    } catch (const std::exception& e) {
        std::cerr << "Error creating directory: " << e.what() << std::endl;
    }
    return false;
}
