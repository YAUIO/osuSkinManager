#pragma once
#include <algorithm>
#include <string>
#include <vector>
#include <filesystem>
#include "SFML/Graphics/Text.hpp"
#include "fmt/core.h"

struct File {
    std::string path;
    std::string name;
    bool isDirectory;

    File(std::string const &pathToFile, std::string const &fileName) {
        path = pathToFile;
        name = fileName;
        isDirectory = false;
    }

    File(std::string const &pathToFile, std::string const &fileName, bool const &is_Directory) {
        path = pathToFile;
        name = fileName;
        isDirectory = is_Directory;
    }
};

struct Files {
    static std::string osuPath;

    static void recordGroup(std::vector<std::vector<File>> & data, std::vector<File> & skins, std::vector<sf::Text> & menu);

    static std::vector<File> getSkins();
};

