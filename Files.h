#pragma once
#include <algorithm>
#include <string>
#include <vector>
#include <filesystem>
#include <fstream>
#include <set>
#include "SFML/Graphics/Text.hpp"
#include "fmt/core.h"
#include "fmt/ranges.h"

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

    void rename(std::string const& newName);
};

struct Files {
    static const std::filesystem::path deps_path;

    static const std::vector<std::string> prefixes;

    static std::string osuPath;

    static void recordGroup(std::vector<std::vector<File>> & data, std::vector<File> & skins, std::vector<sf::Text> & menu, int const& group);

    static void displayGroup(std::vector<std::vector<File>> &data, std::vector<sf::Text> &menu, int const& number);

    static std::vector<File> getSkins();

    static void applyGroups(std::vector<std::vector<File>> &data, bool const& normalize_, std::vector<File> & skins);

    static void revert();

    static std::string getOsuPath();
};

