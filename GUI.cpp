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

void setActiveTextColor(sf::RenderWindow &window, std::vector<sf::Text> &elements, int const& skip) {
    int i = skip;
    while (i < elements.size()) {
        if (elements[i].getFillColor() != sf::Color::Magenta) {
            if (GUI::isCursorOnButton(window, elements[i])) {
                setFillColorDraw(window, elements[i], sf::Color::Green);
            } else {
                setFillColorDraw(window, elements[i], sf::Color::White);
            }
        }else {
            window.draw(elements[i]);
        }
        i++;
    }
}

std::vector<sf::Text>
GUI::getMainGraphics(bool &isListNew, std::vector<sf::Text> buttons, sf::RenderWindow &window, std::vector<File> &files,
                     int const &skip, sf::RectangleShape &rect, sf::RectangleShape const& menuBase) {
    int i = 0;
    int d = 0;

    rect = sf::RectangleShape(sf::Vector2f(20, 20));
    rect.setFillColor(sf::Color::White);
    rect.setPosition(window.getSize().x - rect.getSize().x,
                     static_cast<float>(skip) / static_cast<float>(files.size()) * (window.getSize().y-menuBase.getSize().y));
    window.draw(rect);


    if(isListNew) {
        for (File &skin: files) {
            if (i > skip - 1) {
                auto button = sf::Text(skin.name, font, 22);
                button.setPosition(sf::Vector2f(window.getSize().x / 2 - button.getLocalBounds().width / 2, d * 30));
                buttons.push_back(button);
                d++;
            }
            i++;
        }
        isListNew = false;
    }else{
        for (sf::Text &text : buttons){
            if (i > skip - 1) {
                text.setPosition(sf::Vector2f(window.getSize().x / 2 - text.getLocalBounds().width / 2, d * 30));
                d++;
            }
            i++;
        }
    }

    setActiveTextColor(window, buttons, skip);

    return buttons;
}

void GUI::getMenuGraphics(sf::RenderWindow & window, sf::RectangleShape & base){
    auto menu = std::vector<sf::Text>();
    base = sf::RectangleShape(sf::Vector2f(window.getSize().x,140));
    base.setFillColor(sf::Color::Cyan);
    base.setPosition(0,window.getSize().y-base.getSize().y);
    window.draw(base);

    auto text = std::vector<std::string>{"Next group","Apply","Previous group","Reset","Settings"};

    int i = 0;

    for (std::string const& s : text){
        auto t = sf::Text(s,font,30);
        t.setFillColor(sf::Color::White);
        t.setPosition(sf::Vector2f(20+(i%2)*360,base.getPosition().y+10+(i-i%2)*20));
        window.draw(t);
        i++;
    }
}
