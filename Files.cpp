#include "Files.h"
#include "GUI.h"

template<typename T>
void deleteIndex(std::vector<T> &gameWords, const int &i) {
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

template<typename T>
void replaceIndex(std::vector<T> &gameWords, const int &i, T const &arg) {
    if (i == gameWords.size() - 1) {
        gameWords.pop_back();
        gameWords.push_back(arg);
    } else if (i == 0) {
        auto buf = std::vector(gameWords.begin() + 1, gameWords.end());
        gameWords.clear();
        gameWords.push_back(arg);
        gameWords.insert(gameWords.end(), buf.begin(), buf.end());
    } else {
        auto s = std::vector(gameWords.begin() + i + 1, gameWords.end());
        int x = gameWords.size();
        while (i <= x) {
            gameWords.pop_back();
            x--;
        }
        gameWords.push_back(arg);
        gameWords.insert(gameWords.end(), s.begin(), s.end());
    }
}

void deleteIndex(std::vector<sf::Text> &gameWords, const int &i) {
    try {
        if (i == gameWords.size() - 1) {
            gameWords.pop_back();
        } else if (i == 0) {
            gameWords = std::vector(gameWords.begin() + 1, gameWords.end());
        } else {
            auto s = std::vector<sf::Text>();

            s = std::vector(gameWords.begin() + i + 1, gameWords.end());

            int x = static_cast<int>(gameWords.size());
            while (i < x) {
                gameWords.pop_back();
                x--;
            }

            gameWords.insert(gameWords.end(), s.begin(), s.end());
        }
    } catch (std::exception &e) {
        fmt::println("deleteIndex error, {}", e.what());
    }
}

void File::rename(std::string const &newName) {
    if (newName != name) {
        name = newName;
        std::string oldPath = path;
        if (path.contains('/')) {
            int slash = static_cast<int>(path.find_last_of('/') + 1);
            if (slash == path.size()) {
                slash = static_cast<int>(path.substr(0, path.size() - 1).find_last_of('/')) + 1;
            }
            path = path.substr(0, slash) + name;
        } else {
            int slash = static_cast<int>(path.find_last_of('\\') + 1);
            if (slash == path.size()) {
                slash = static_cast<int>(path.substr(0, path.size() - 1).find_last_of('\\')) + 1;
            }
            path = path.substr(0, slash) + name;
        }
        std::filesystem::rename(oldPath, path);
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

void Files::recordGroup(std::vector<std::vector<File> > &data, std::vector<File> &skins, std::vector<sf::Text> &menu,
                        int const &group) {
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

    i = 0;

    for (int const &a: deleteIndexes) {
        deleteIndex(menu, a - i);
        i++;
    }

    for (std::string &skin: stringSkins) {
        for (File &file: skins) {
            if (file.name == skin) {
                selectedSkins.push_back(file);
                break;
            }
        }
    }


    if (group == data.size()) {
        data.push_back(selectedSkins);
    } else {
        try {
            replaceIndex(data, group, selectedSkins);
        } catch (std::exception &e) {
            fmt::println("exception in replaceIndex, {}", e.what());
        }
    }
}

void Files::displayGroup(std::vector<std::vector<File> > &data, std::vector<sf::Text> &menu, int const &number) {
    auto map = std::map<std::string, sf::Text *>();
    auto dataMap = std::map<std::string, File *>();

    for (File &f: data[number]) {
        dataMap[f.name] = &f;
    }

    for (sf::Text &t: menu) {
        map[t.getString()] = &t;
    }

    for (File &f: data[number]) {
        if (!map.contains(f.name)) {
            auto text = sf::Text(f.name, GUI::font, 22);
            text.setFillColor(sf::Color::Magenta);
            menu.push_back(text);
        } else {
            map[f.name]->setFillColor(sf::Color::White);
        }
    }

    for (sf::Text &t: menu) {
        if (dataMap.contains(t.getString())) {
            t.setFillColor(sf::Color::Magenta);
        } else {
            t.setFillColor(sf::Color::White);
        }
    }

    std::ranges::sort(menu.begin(), menu.end(),
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
                      [](sf::Text const &f) { return f.getString(); });
}

bool normalize(File &f) {

    std::string buf;
    int ch = 0;
    for (char const &c: f.name) {
        if ((static_cast<int>(c) > 64 && static_cast<int>(c) < 91) ||
            (static_cast<int>(c) > 96 && static_cast<int>(c) < 123)) {
            break;
        }
        ch++;
    }

    if (ch == f.name.size()) {
        ch = 0;
        for (char const &c: f.name) {
            if (static_cast<int>(c) > 47 && static_cast<int>(c) < 58) {
                break;
            }
            ch++;
        }
    }

    if (f.name.substr(ch) == f.name || ch == f.name.size()) {
        return false;
    }

    f.rename(f.name.substr(ch));
    return true;
}

void writeBackup(std::vector<File> const &from, std::vector<File> const &to) {
    std::fstream backup;
    std::string dir = "deps/backup.txt";
    backup.open(dir, std::ios::out);

    int i = 0;
    while (i < from.size()) {
        backup << fmt::format("{} <- {}\n", to[i].name, from[i].name);
        i++;
    }

    backup.close();
}

void Files::revert() {
    std::fstream backup;
    std::string dir = "deps/backup.txt";
    backup.open(dir, std::ios::in);
    auto line = std::string();

    while (std::getline(backup, line)) {
        auto f = File(osuPath + line.substr(0, line.find('<')), line.substr(0, line.find('<')), true);
        f.rename(line.substr(line.find(" <- ") + 4));
    }
}

void Files::applyGroups(std::vector<std::vector<File>> &data, bool const &normalize_, std::vector<File> &skins) {
    int i = 0;
    int v = 0;
    auto indexes = std::vector<std::vector<int>>();
    bool lastRes = true;

    auto origSkins = skins;

    auto from = std::vector<File>();
    auto to = std::vector<File>();

    for (std::vector<File> &vf: data) {
        indexes.push_back(std::vector<int>());
        for (File &f: vf) {
            i = 0;
            for (File &s: skins) {
                if (f.name == s.name) {
                    break;
                }
                i++;
            }
            indexes[v].push_back(i);
        }
        v++;
    }

    i = 0;
    while (i < indexes.size()) {
        v = 0;
        while (v < indexes[i].size()) {
            from.push_back(skins[indexes[i][v]]);
            if (normalize_) {
                lastRes = normalize(skins[indexes[i][v]]);
            }
            skins[indexes[i][v]].rename(prefixes[i] + skins[indexes[i][v]].name);
            to.push_back(skins[indexes[i][v]]);
            v++;
        }
        i++;
    }

    if (normalize_) {
        i = 0;
        v = 0;
        auto setInd = std::set<int>();
        for (std::vector<int> &vec: indexes) {
            for (int &z: vec) {
                setInd.insert(z);
            }
        }


        while (i < skins.size()) {
            if (!setInd.contains(i)) {
                from.push_back(skins[i]);
                lastRes = normalize(skins[i]);
                if(lastRes){
                    to.push_back(skins[i]);
                }else{
                    from.pop_back();
                }
            }
            i++;
        }
    }

    writeBackup(from, to);
}

