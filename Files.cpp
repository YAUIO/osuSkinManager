#include "Files.h"

template<typename T>
void deleteIndex(std::vector<T> &gameWords, const int &i) {
    fmt::println("{}","call to the template function, expect errors");
    if (i == gameWords.size() - 1) {
        gameWords.pop_back();
    } else {
        auto s = std::vector(gameWords.begin() + i, gameWords.end());
        int x = 0;
        while (x >= i) {
            gameWords.pop_back();
            x++;
        }
        s.insert(s.end(), gameWords.begin(), gameWords.end());
        gameWords = s;
    }
}

void deleteIndex(std::vector<sf::Text> &gameWords, const int &i) {
    if (i == gameWords.size() - 1) {
        gameWords.pop_back();
    } else if (i == 0) {
        gameWords = std::vector(gameWords.begin() + 1, gameWords.end());
    } else {
        auto s = std::vector(gameWords.begin() + i + 1, gameWords.end());
        int x = gameWords.size();
        while (i < x) {
            gameWords.pop_back();
            x--;
        }
        gameWords.insert(gameWords.end(), s.begin(), s.end());
    }

}

std::vector<File> Files::getSkins() {
    auto skins = std::vector<File>();

    for (const auto &entry: std::filesystem::directory_iterator(Files::osuPath)) {
        if (entry.is_directory()) {
            skins.push_back(
                    File(entry.path().generic_string(), entry.path().filename().generic_string(), true));
        }
    }

    std::ranges::sort(skins.begin(), skins.end(),
                      [](std::string const &first, std::string const &second) {
                          int i = 0;
                          for (char c: first) {
                              if (i >= second.size()) {
                                  return true;
                              }
                              if (c < second[i]) {
                                  return true;
                              } else if (c > second[i]) {
                                  return false;
                              }
                              i++;
                          }
                          return true;
                      },
                      [](File const &f) { return f.name; });

    return skins;
}

void Files::recordGroup(std::vector<std::vector<File>> &data, std::vector<File> &skins, std::vector<sf::Text> &menu) {
    auto stringSkins = std::vector<std::string>();
    auto selectedSkins = std::vector<File>();
    auto deleteIndexes = std::vector<int>();
    int i = 0;

    for (sf::Text &button: menu) {
        if (button.getFillColor() == sf::Color::Magenta) {
            stringSkins.push_back(button.getString());
            deleteIndexes.push_back(i);
        }
        i++;
    }

    for (int const &a: deleteIndexes) {
        deleteIndex(menu, a);
    }

    for (std::string &skin: stringSkins) {
        for (File &file: skins) {
            if (file.name == skin) {
                selectedSkins.push_back(file);
                break;
            }
        }
    }

    data.push_back(selectedSkins);
}
