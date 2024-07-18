#include <fmt/core.h>
#include <fmt/ranges.h>
#include <SFML/Graphics.hpp>
#include "Files.h"
#include "GUI.h"

std::string Files::osuPath = "/home/yudek/osu/Skins";
sf::Font GUI::font;
int main() {
    GUI::font.loadFromFile("deps/font.ttf");

    auto window = sf::RenderWindow(sf::VideoMode(900, 600), "SkinManager");
    window.setFramerateLimit(240);

    sf::Event event;
    bool isSkinListReady = false;
    auto skins = std::vector<File>();
    int skip = 0;

    while (window.isOpen()) {
        window.clear(sf::Color::Black);

        if(!isSkinListReady) {
            skins = Files::getSkins();
            isSkinListReady = true;
        }

        auto gui = GUI::getMainGraphics(window, skins, skip);

        while (window.pollEvent(event)) {
            if(event.type == sf::Event::Closed) {
                window.close();
            }

            if(event.type == sf::Event::MouseWheelScrolled) {
                skip -= static_cast<int>(static_cast<double>(event.mouseWheelScroll.delta)*1.5);
                if(skip<0) {
                    skip = 0;
                }
                if(skip>skins.size()-6) {
                    skip = skins.size()-6;
                }
            }
        }

        window.display();
    }
}
