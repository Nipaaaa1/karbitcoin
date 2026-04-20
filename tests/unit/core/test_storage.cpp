#include <gtest/gtest.h>
#include <nlohmann/json.hpp>
#include <filesystem>
#include <fstream>
#include "core/storage.hpp"

namespace fs = std::filesystem;

class StorageTest : public ::testing::Test {
protected:
    std::string test_dir = "test_data";
    std::string test_file = "test_data/test.json";

    void SetUp() override {
        if (!fs::exists(test_dir)) {
            fs::create_directory(test_dir);
        }
    }

    void TearDown() override {
        if (fs::exists(test_dir)) {
            fs::remove_all(test_dir);
        }
    }
};

TEST_F(StorageTest, SaveAndLoadJson) {
    nlohmann::json original_json = {
        {"key", "value"},
        {"number", 123},
        {"array", {1, 2, 3}}
    };

    EXPECT_TRUE(Storage::saveJson(test_file, original_json));
    
    nlohmann::json loaded_json = Storage::loadJson(test_file);
    
    EXPECT_EQ(original_json, loaded_json);
}

TEST_F(StorageTest, LoadNonExistentFile) {
    nlohmann::json loaded_json = Storage::loadJson("non_existent.json");
    EXPECT_TRUE(loaded_json.is_null());
}

TEST_F(StorageTest, SaveToNonExistentDirectory) {
    nlohmann::json j = {{"test", 1}};
    // Should fail if parent directory doesn't exist and we don't handle it
    // But a good implementation might create it.
    EXPECT_TRUE(Storage::saveJson("nested/dir/test.json", j));
    EXPECT_TRUE(fs::exists("nested/dir/test.json"));
    fs::remove_all("nested");
}
