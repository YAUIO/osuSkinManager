#pragma once
#include <algorithm>
#include <string>
#include <vector>
#include <filesystem>

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

    static std::vector<File> getSkins();
};

