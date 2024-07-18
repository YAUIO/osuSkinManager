#include "GUI.h"

std::vector<sf::Text> GUI::getMainGraphics(sf::RenderWindow & window, std::vector<File> & files, int const& skip){
    auto buttons = std::vector<sf::Text>();
    int i = 0;
    int d = 0;

    auto rect = sf::RectangleShape(sf::Vector2f(20,20));
    rect.setFillColor(sf::Color::White);
    rect.setPosition(window.getSize().x-rect.getSize().x,static_cast<float>(skip)/static_cast<float>(files.size())*window.getSize().y);
    window.draw(rect);

    for (File & skin : files) {
        auto button = sf::Text(skin.name,font,22);
        button.setPosition(sf::Vector2f(window.getSize().x/2-button.getLocalBounds().width/2,d*30));
        if (i>skip-1) {
            window.draw(button);
            d++;
        }
        buttons.push_back(button);
        i++;
    }

    return buttons;
}
