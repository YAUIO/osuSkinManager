#include <fmt/core.h>
#include <fmt/ranges.h>
#include <SFML/Graphics.hpp>
#include "Files.h"
#include "GUI.h"

//std::string Files::osuPath = "/home/yudek/osu/Skins";
std::string Files::osuPath = "G:\\osu!\\Skins";
sf::Font GUI::font;
int main() {
    GUI::font.loadFromFile("deps/font.ttf");

    auto window = sf::RenderWindow(sf::VideoMode(900, 600), "SkinManager");
    window.setFramerateLimit(240);

    sf::Event event;
    sf::RectangleShape rect;
    bool isSkinListReady = false;
    bool trackCursor = false;
    auto skins = std::vector<File>();
    int skip = 0;

    while (window.isOpen()) {
        window.clear(sf::Color::Black);

        if(!isSkinListReady) {
            try {
                skins = Files::getSkins();
            }catch (std::exception & e){
                window.close();
                fmt::println("{}",e.what());
            }
            isSkinListReady = true;
        }

        auto gui = GUI::getMainGraphics(window, skins, skip, rect);

        if(trackCursor){
            int y = sf::Mouse::getPosition(window).y;
            if(y<window.getSize().y-rect.getSize().y && y>0) {
                rect.setPosition(rect.getPosition().x, y);
                skip = static_cast<int>(static_cast<float>(skins.size()) *
                                        (static_cast<float>(y) / static_cast<float>(window.getSize().y)));
            }
        }

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

            if(event.type == sf::Event::MouseButtonPressed && window.getSize().x - sf::Mouse::getPosition(window).x <= 40){
                trackCursor = true;
            }

            if(event.type == sf::Event::MouseButtonReleased){
                trackCursor = false;
            }
        }

        window.display();
    }
}
