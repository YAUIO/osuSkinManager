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
    bool isSkinListNew = true;
    bool trackCursor = false;
    bool shiftSelection = false;
    auto skins = std::vector<File>();
    auto gui = std::vector<sf::Text>();
    int skip = 0;
    int lastSel = -1;
    bool lastSelected = true;

    while (window.isOpen()) {
        window.clear(sf::Color::Black);

        if (!isSkinListReady) {
            try {
                skins = Files::getSkins();
            } catch (std::exception &e) {
                window.close();
                fmt::println("{}", e.what());
            }
            isSkinListReady = true;
        }

        gui = GUI::getMainGraphics(isSkinListNew, gui, window, skins, skip, rect);

        if (trackCursor) {
            int y = sf::Mouse::getPosition(window).y;
            if (y < window.getSize().y - rect.getSize().y && y > 0) {
                rect.setPosition(rect.getPosition().x, y);
                skip = static_cast<int>(static_cast<float>(skins.size()) *
                                        (static_cast<float>(y) / static_cast<float>(window.getSize().y)));
            }
        }

        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }

            if (event.type == sf::Event::MouseWheelScrolled) {
                skip -= static_cast<int>(static_cast<double>(event.mouseWheelScroll.delta) * 1.5);
                if (skip < 0) {
                    skip = 0;
                }
                if (skip > skins.size() - 18) {
                    skip = skins.size() - 18;
                }
            }

            if (event.type == sf::Event::KeyPressed) {
                if (event.key.code == sf::Keyboard::Key::LShift) {
                    shiftSelection = true;
                }
            }

            if (event.type == sf::Event::KeyReleased) {
                if (event.key.code == sf::Keyboard::Key::LShift) {
                    shiftSelection = false;
                }
            }

            if (event.type == sf::Event::MouseButtonPressed) {
                if (window.getSize().x - sf::Mouse::getPosition(window).x <= 40) {
                    trackCursor = true;
                } else {
                    if (!shiftSelection) {
                        int i = 0;
                        for (sf::Text &button: gui) {
                            if (GUI::isCursorOnButton(window, button)) {
                                if (button.getFillColor() == sf::Color::Magenta) {
                                    button.setFillColor(sf::Color::White);
                                    lastSelected = false;
                                } else {
                                    button.setFillColor(sf::Color::Magenta);
                                    lastSelected = true;
                                }
                                lastSel = i;
                                break;
                            }
                            i++;
                        }
                    } else {
                        int first = 0;
                        for (sf::Text &button: gui) {
                            if (GUI::isCursorOnButton(window, button)) {
                                if (button.getFillColor() == sf::Color::Magenta) {
                                    button.setFillColor(sf::Color::White);
                                } else {
                                    button.setFillColor(sf::Color::Magenta);
                                }
                                break;
                            }
                            first++;
                        }
                        int buf = -1;
                        if(lastSel!=-1) {
                            buf = first;
                            if (first < lastSel){
                                while (first < lastSel){
                                    if (lastSelected){
                                        gui[first].setFillColor(sf::Color::Magenta);
                                    }else{
                                        gui[first].setFillColor(sf::Color::White);
                                    }
                                    first++;
                                }
                            }else{
                                while (first > lastSel){
                                    if (lastSelected){
                                        gui[first].setFillColor(sf::Color::Magenta);
                                    }else{
                                        gui[first].setFillColor(sf::Color::White);
                                    }
                                    first--;
                                }
                            }
                        }
                        lastSel = buf;
                        first = 0;
                    }
                }
            }

            if (event.type == sf::Event::MouseButtonReleased) {
                trackCursor = false;
            }
        }

        window.display();
    }

}
