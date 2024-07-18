#include <fmt/core.h>
#include <fmt/ranges.h>
#include <SFML/Graphics.hpp>
#include "Files.h"

std::string f::Files::osuPath = "/home/yudek/osu/Skins";
int main() {
    auto window = sf::RenderWindow(sf::VideoMode(900, 600), "SkinManager");
    window.setFramerateLimit(240);

    sf::Event event;
    bool isSkinListReady = false;

    while (window.isOpen()) {
        window.clear(sf::Color::Black);

        if(!isSkinListReady) {
            auto skins = f::Files::getSkins();
            isSkinListReady = true;
        }

        while (window.pollEvent(event)) {
            if(event.type == sf::Event::Closed) {
                window.close();
            }
        }

        window.display();
    }
}
