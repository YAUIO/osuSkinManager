#pragma once
#include <vector>
#include "SFML/Graphics.hpp"
#include "Files.h"

struct GUI {
    static sf::Font font;
    static bool isCursorOnButton(sf::RenderWindow &window, sf::Text const &button1);
    static bool isCursorOnButton(sf::RenderWindow &window, sf::RectangleShape const &button1);
    static std::vector<sf::Text> getMainGraphics(sf::RenderWindow & window, std::vector<File> & files, int const& skip, sf::RectangleShape & rect);
};
