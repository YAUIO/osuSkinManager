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

bool GUI::isCursorOnButton(sf::RenderWindow &window, sf::Sprite const &button1) {
    return (sf::Mouse::getPosition(window).x - (button1.getPosition().x) <=
            button1.getGlobalBounds().width) &&
           (sf::Mouse::getPosition(window).x - (button1.getPosition().x + button1.getGlobalBounds().width) >=
            button1.getGlobalBounds().width * -1) &&
           (sf::Mouse::getPosition(window).y - (button1.getPosition().y) <=
            button1.getGlobalBounds().height) &&
           (sf::Mouse::getPosition(window).y - (button1.getPosition().y + button1.getGlobalBounds().height) >=
            button1.getGlobalBounds().height * -1);
}

void setFillColorDraw(sf::RenderWindow &window, sf::Text &text, sf::Color const &color) {
    text.setFillColor(color);
    window.draw(text);
}

void setFillColorDraw(sf::RenderWindow &window, sf::Sprite &panel, sf::Texture const &texture) {
    panel.setTexture(texture);
    window.draw(panel);
}

void GUI::setActiveTextColor(sf::RenderWindow &window, std::vector<sf::Text> &elements, int const &skip) {
    int i = skip;
    while (i < elements.size()) {
        if (elements[i].getFillColor() != sf::Color::Magenta) {
            if (GUI::isCursorOnButton(window, elements[i])) {
                setFillColorDraw(window, elements[i], sf::Color::Green);
            } else {
                setFillColorDraw(window, elements[i], sf::Color::White);
            }
        } else {
            window.draw(elements[i]);
        }
        i++;
    }
}

void GUI::setActiveTextColor(sf::RenderWindow &window, std::vector<sf::Text> &elements) {
    int i = 0;
    while (i < elements.size()) {
        if (elements[i].getFillColor() != sf::Color::Magenta) {
            if (GUI::isCursorOnButton(window, elements[i])) {
                setFillColorDraw(window, elements[i], sf::Color::Green);
            } else {
                setFillColorDraw(window, elements[i], sf::Color::White);
            }
        } else {
            window.draw(elements[i]);
        }
        i++;
    }
}

void setActivePanelColor(sf::RenderWindow &window, std::vector<sf::Sprite> &elements, int const &skip,
                         sf::Texture const &textIdle, sf::Texture const &textActive) {
    int i = skip;
    while (i < elements.size()) {
        if (GUI::isCursorOnButton(window, elements[i])) {
            setFillColorDraw(window, elements[i], textActive);
        } else {
            setFillColorDraw(window, elements[i], textIdle);
        }
        i++;
    }
}

void
getMainBase(std::vector<sf::Sprite> &vec, std::vector<sf::Text> &mainVec, sf::RenderWindow &window, int const &skip,
            sf::Texture const &textIdle, sf::Texture const &textActive) {
    vec.clear();
    for (sf::Text &t: mainVec) {
        sf::Sprite sprite = sf::Sprite(textIdle);
        sprite.setPosition(
                sf::Vector2f(window.getSize().x / 2 - sprite.getGlobalBounds().width / 2, t.getPosition().y));
        vec.push_back(sprite);
    }

    setActivePanelColor(window, vec, skip, textIdle, textActive);
}

void
GUI::getMainGraphics(bool &isListNew, std::vector<sf::Sprite> &vec, std::vector<sf::Text> &buttons,
                     sf::RenderWindow &window, std::vector<File> &files,
                     int const &skip, sf::RectangleShape &rect, sf::RectangleShape const &menuBase,
                     sf::Texture const &textIdle, sf::Texture const &textActive) {
    int i = 0;
    int d = 0;

    rect = sf::RectangleShape(sf::Vector2f(20, 20));
    rect.setFillColor(sf::Color::White);
    rect.setPosition(window.getSize().x - rect.getSize().x,
                     static_cast<float>(skip) / static_cast<float>(files.size()) *
                     (window.getSize().y - menuBase.getSize().y));
    window.draw(rect);


    if (isListNew) {
        for (File &skin: files) {
            if (i > skip - 1) {
                auto button = sf::Text(skin.name, font, 22);
                button.setPosition(sf::Vector2f(window.getSize().x / 2 - button.getLocalBounds().width / 2, d * 34));
                buttons.push_back(button);
                d++;
            }
            i++;
        }
        isListNew = false;
    } else {
        for (sf::Text &text: buttons) {
            if (i > skip - 1) {
                text.setPosition(sf::Vector2f(window.getSize().x / 2 - text.getLocalBounds().width / 2, d * 34));
                d++;
            }
            i++;
        }
    }

    getMainBase(vec, buttons, window, skip, textIdle,textActive);

    setActiveTextColor(window, buttons, skip);
}


void GUI::getMenuGraphics(bool &init, sf::RenderWindow &window, sf::RectangleShape &base, std::vector<sf::Text> &menu,
                          int const &counter) {
    base = sf::RectangleShape(sf::Vector2f(window.getSize().x, 140));
    base.setFillColor(sf::Color::Cyan);
    base.setPosition(0, window.getSize().y - base.getSize().y);
    window.draw(base);

    if (init) {
        menu.clear();

        auto text = std::vector<std::string>{"Next group", "Apply", "Previous group", "Reset", "Settings", "Revert last change"};

        int i = 0;

        for (std::string const &s: text) {
            auto t = sf::Text(s, font, 30);
            t.setFillColor(sf::Color::White);
            t.setPosition(sf::Vector2f(20 + (i % 2) * 360, base.getPosition().y + 10 + (i - i % 2) * 20));
            menu.push_back(t);
            i++;
        }

        auto c = sf::Text(std::to_string(counter + 1), font, 60);
        c.setFillColor(sf::Color::White);
        c.setPosition(sf::Vector2f(base.getSize().x - 40 - c.getGlobalBounds().width, base.getPosition().y + 8));

        menu.push_back(c);

        init = false;
    }

    menu[menu.size() - 1].setString(std::to_string(counter + 1));
    menu[menu.size() - 1].setPosition(
            sf::Vector2f(base.getSize().x - 40 - menu[menu.size() - 1].getGlobalBounds().width,
                         base.getPosition().y + 8));

    setActiveTextColor(window, menu);
}

void GUI::getSettings(bool &viewChanged, sf::RenderWindow &window, std::vector<sf::Text> &settings) {
    if (viewChanged && settings.empty()) {
        settings = std::vector<sf::Text>();

        auto text = std::vector<std::string>{"osu! path", "resolution"/*, "normalize"*/};

        int i = 0;

        for (std::string const &s: text) {
            auto t = sf::Text(s, font, 40);
            t.setFillColor(sf::Color::White);
            t.setPosition(sf::Vector2f(window.getSize().x / 2 - t.getGlobalBounds().width / 2, i * 60 + 10));
            settings.push_back(t);
            i++;
        }

        viewChanged = false;
    }

    setActiveTextColor(window, settings);
}

void GUI::getOsuPath(sf::RenderWindow &window){
    auto t = sf::Text(Files::osuPath,font,22);
    t.setFillColor(sf::Color::White);
    t.setPosition(window.getSize().x/2-t.getGlobalBounds().width/2,window.getSize().y/2-t.getGlobalBounds().height/2);

    auto x = sf::Text("Put skin folder path in the first line of config.txt and press Enter",font,22);
    x.setFillColor(sf::Color::White);
    x.setPosition(window.getSize().x/2-x.getGlobalBounds().width/2,window.getSize().y/4-x.getGlobalBounds().height/2);

    window.draw(x);
    window.draw(t);
}
