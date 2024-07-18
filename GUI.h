#pragma once
#include <vector>
#include "SFML/Graphics.hpp"
#include "Files.h"

struct GUI {
    static sf::Font font;
    static std::vector<sf::Text> getMainGraphics(sf::RenderWindow & window, std::vector<File> & files, int const& skip);
};
