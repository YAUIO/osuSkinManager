#include "GUI.h"

bool GUI::isCursorOnButton(sf::RenderWindow &window, sf::Text const &button1) {
    return (sf::Mouse::getPosition(window).x - (button1.getPosition().x) <=
            button1.getGlobalBounds().width) &&
           (sf::Mouse::getPosition(window).x - (button1.getPosition().x + button1.getGlobalBounds().width) >=
            button1.getGlobalBounds().width * -1) &&
           (sf::Mouse::getPosition(window).y - (button1.getPosition().y) <=
            button1.getGlobalBounds().height) &&
           (sf::Mouse::getPosition(window).y - (button1.getPosition().y + button1.getGlobalBounds().height) >=
            button1.getGlobalBounds().height * -1);
}

bool GUI::isCursorOnButton(sf::RenderWindow &window, sf::RectangleShape const &button1) {
    return (sf::Mouse::getPosition(window).x - (button1.getPosition().x) <=
            button1.getSize().x) &&
           (sf::Mouse::getPosition(window).x - (button1.getPosition().x + button1.getSize().x) >=
            button1.getSize().x * -1) &&
           (sf::Mouse::getPosition(window).y - (button1.getPosition().y) <=
            button1.getSize().y) &&
           (sf::Mouse::getPosition(window).y - (button1.getPosition().y + button1.getSize().y) >=
            button1.getSize().y * -1);
}

void setFillColorDraw(sf::RenderWindow &window, sf::Text &text, sf::Color const &color) {
    text.setFillColor(color);
    window.draw(text);
}

void setActiveTextColor(sf::RenderWindow &window, std::vector<sf::Text> &elements) {
    int i = 0;
    while (i < elements.size()) {
        if (GUI::isCursorOnButton(window, elements[i])) {
            setFillColorDraw(window, elements[i], sf::Color::Green);
        } else {
            setFillColorDraw(window, elements[i], sf::Color::White);
        }
        i++;
    }
}

std::vector<sf::Text> GUI::getMainGraphics(sf::RenderWindow & window, std::vector<File> & files, int const& skip, sf::RectangleShape & rect){
    auto buttons = std::vector<sf::Text>();
    int i = 0;
    int d = 0;

    rect = sf::RectangleShape(sf::Vector2f(20,20));
    rect.setFillColor(sf::Color::White);
    rect.setPosition(window.getSize().x-rect.getSize().x,static_cast<float>(skip)/static_cast<float>(files.size())*window.getSize().y);
    window.draw(rect);

    for (File & skin : files) {
        if (i>skip-1) {
            auto button = sf::Text(skin.name,font,22);
            button.setPosition(sf::Vector2f(window.getSize().x/2-button.getLocalBounds().width/2,d*30));
            window.draw(button);
            buttons.push_back(button);
            d++;
        }
        i++;
    }

    setActiveTextColor(window,buttons);

    return buttons;
}
