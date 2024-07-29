#pragma once
#include <vector>
#include "SFML/Graphics.hpp"
#include "Files.h"

struct GUI {
    static sf::Font font;
    static bool isCursorOnButton(sf::RenderWindow &window, sf::Text const &button1);
    static bool isCursorOnButton(sf::RenderWindow &window, sf::RectangleShape const &button1);
    static void getMainGraphics(bool & isListNew, std::vector<sf::Text> & buttons, sf::RenderWindow & window, std::vector<File> & files, int const& skip, sf::RectangleShape & rect, sf::RectangleShape const& menuBase);
    static void getMenuGraphics(bool & init,sf::RenderWindow & window, sf::RectangleShape & base, std::vector<sf::Text> & menu, int const& counter);
    static void getSettings(bool & viewChanged,sf::RenderWindow & window, std::vector<sf::Text> & settings);
    static void setActiveTextColor(sf::RenderWindow &window, std::vector<sf::Text> &elements);
    static void setActiveTextColor(sf::RenderWindow &window, std::vector<sf::Text> &elements, int const& skip);
};
