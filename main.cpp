#include <fmt/core.h>
#include <fmt/ranges.h>
#include <SFML/Graphics.hpp>
#include "Files.h"
#include "GUI.h"

//std::string Files::osuPath = "testSample/";
std::string Files::osuPath;
sf::Font GUI::font;
const std::vector<std::string> Files::prefixes = std::vector<std::string>{"!-", "#-", "$-", "*-", "+-", "@-", "^-",
                                                                          "`-", "{-", "|-", "}-", "~-"};
const std::vector<char> alphabet = {
        'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o',
        'p',
        'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z'
};

const std::filesystem::path Files::deps_path = std::filesystem::current_path().parent_path() / "deps/";

int main() {
    GUI::font.loadFromFile(Files::deps_path.string().append("font.ttf"));
    sf::Texture textIdle;
    sf::Texture textActive;
    textIdle.loadFromFile(Files::deps_path.string().append("sfml.png"));
    textActive.loadFromFile(Files::deps_path.string().append("sfml.png"));

    bool proceed = false;
    bool osuPath = false;
    bool closed = false;

    {
        auto warning = sf::RenderWindow(sf::VideoMode(800, 900), "SkinManager");
        warning.setFramerateLimit(240);
        auto b1 = sf::Sprite(textIdle);
        auto b2 = sf::Sprite(textIdle);

        auto warText = sf::Text(
                "This program is in beta-testing\nIt can break your skins' names\nBe aware that it will wipe prefixes after clicking apply",
                GUI::font, 30);
        warText.setFillColor(sf::Color::White);
        auto agree = sf::Text("I agree", GUI::font, 22);
        agree.setFillColor(sf::Color::White);
        agree.setPosition(warning.getSize().x / 3 - agree.getGlobalBounds().width / 2, warning.getSize().y / 4 * 3);
        auto exit = sf::Text("Exit", GUI::font, 22);
        exit.setFillColor(sf::Color::White);
        exit.setPosition(warning.getSize().x / 3 * 2 - exit.getGlobalBounds().width / 2, warning.getSize().y / 4 * 3);
        auto vecC = std::vector<sf::Text>{agree, exit};
        auto e = sf::Event();
        b1.setPosition(agree.getPosition().x - b1.getGlobalBounds().width / 3, agree.getPosition().y);
        b2.setPosition(exit.getPosition().x - b2.getGlobalBounds().width / 3, exit.getPosition().y);

        while (warning.isOpen()) {
            warning.clear(sf::Color::Black);
            warning.draw(warText);
            warning.draw(b1);
            warning.draw(b2);
            GUI::setActiveTextColor(warning, vecC);

            while (warning.pollEvent(e)) {
                if (e.type == sf::Event::Closed) {
                    warning.close();
                }

                if (e.type == sf::Event::MouseButtonPressed) {
                    if (GUI::isCursorOnButton(warning, agree)) {
                        warning.close();
                        proceed = true;
                    }

                    if (GUI::isCursorOnButton(warning, exit)) {
                        warning.close();
                    }
                }
            }
            warning.display();
        }
    }

    if (proceed) {

        sf::Event event;

        while (!osuPath) {

            auto window = sf::RenderWindow(sf::VideoMode(800, 900), "SkinManager");
            window.setPosition(sf::Vector2i(960 - window.getSize().x / 2, 540 - window.getSize().y / 2));
            window.setFramerateLimit(240);

            if (Files::getOsuPath().empty()) {
                while (window.isOpen()) {
                    window.clear(sf::Color::Black);
                    GUI::getOsuPath(window);

                    while (window.pollEvent(event)) {
                        if (event.type == sf::Event::KeyPressed) {
                            if (event.key.code == sf::Keyboard::Enter) {
                                Files::osuPath = Files::getOsuPath();
                                if (Files::osuPath.contains("Skins")) {
                                    osuPath = true;
                                    window.close();
                                } else {
                                    Files::osuPath.clear();
                                }
                            }
                        } else if (event.type == sf::Event::Closed) {
                            window.close();
                            proceed = false;
                            osuPath = true;
                        }
                    }

                    window.display();
                }
            } else {
                Files::osuPath = Files::getOsuPath();
                osuPath = true;
                if (Files::osuPath.contains('\\')) {
                    if (!Files::osuPath.ends_with('\\')) {
                        Files::osuPath.append("\\");
                    }
                } else if (Files::osuPath.contains('/')) {
                    if (!Files::osuPath.ends_with('/')) {
                        Files::osuPath.append("/");
                    }
                }
            }
        }

        if (proceed) {

            while (!closed) {

                auto window = sf::RenderWindow(sf::VideoMode(800, 900), "SkinManager");
                window.setPosition(sf::Vector2i(960 - window.getSize().x / 2, 540 - window.getSize().y / 2));
                window.setFramerateLimit(240);

                sf::RectangleShape rect;
                sf::RectangleShape menuBase;
                bool init = true;
                bool isSkinListReady = false;
                bool isSkinListNew = true;
                bool trackCursor = false;
                bool viewChanged = false;
                bool shiftSelection = false;
                bool settings = false;
                auto skins = std::vector<File>();
                auto records = std::vector<std::vector<File>>();
                auto gui = std::vector<sf::Text>();
                auto guiBase = std::vector<sf::Sprite>();
                auto menu = std::vector<sf::Text>();
                auto sett = std::vector<sf::Text>();
                int skip = 0;
                int lastSel = -1;
                int displayingGroup = 0;
                bool lastSelected = true;
                bool normalize = false;

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

                    if (!settings) {
                        GUI::getMainGraphics(isSkinListNew, guiBase, gui, window, skins, skip, rect, menuBase, textIdle,
                                             textActive);
                    } else {
                        GUI::getSettings(viewChanged, window, sett);
                    }
                    GUI::getMenuGraphics(init, window, menuBase, menu, displayingGroup);

                    if (trackCursor) {
                        int y = sf::Mouse::getPosition(window).y;
                        if (y < window.getSize().y - menuBase.getSize().y - rect.getSize().y && y > 0) {
                            skip = static_cast<int>(static_cast<float>(skins.size()) *
                                                    (static_cast<float>(y) /
                                                     static_cast<float>(window.getSize().y - menuBase.getSize().y)));
                        }
                    }

                    try {

                        while (window.pollEvent(event)) {
                            if (event.type == sf::Event::Closed) {
                                window.close();
                                closed = true;
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
                                        if (sf::Mouse::getPosition(window).y <
                                            (window.getSize().y - static_cast<int>(menuBase.getSize().y))) {
                                            if (!settings) {
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
                                                /*for (sf::Text & button : sett){
                                                    if (GUI::isCursorOnButton(window, button)) {
                                                        if (button.getString() == "normalize") {
                                                            if(button.getFillColor() == sf::Color::Magenta || normalize){
                                                                button.setFillColor(sf::Color::White);
                                                            }else {
                                                                button.setFillColor(sf::Color::Magenta);
                                                            }

                                                            normalize = !normalize;
                                                        }
                                                    }
                                                }*/
                                            }
                                        } else {
                                            for (sf::Text &button: menu) {
                                                try {
                                                    if (GUI::isCursorOnButton(window, button)) {
                                                        if (button.getString() == "Settings") {
                                                            settings = !settings;
                                                            if (settings) {
                                                                button.setFillColor(sf::Color::Magenta);
                                                                viewChanged = true;
                                                            } else {
                                                                button.setFillColor(sf::Color::White);
                                                            }
                                                        } else if (button.getString() == "Next group") {
                                                            if (displayingGroup < Files::prefixes.size()) {
                                                                Files::recordGroup(records, skins, gui,
                                                                                   displayingGroup);
                                                                displayingGroup++;
                                                                if (displayingGroup != records.size()) {
                                                                    Files::displayGroup(records, gui, displayingGroup);
                                                                }
                                                            }
                                                        } else if (button.getString() == "Previous group") {
                                                            if (displayingGroup > 0) {
                                                                displayingGroup--;
                                                                Files::displayGroup(records, gui, displayingGroup);
                                                            }
                                                        } else if (button.getString() == "Apply") {
                                                            if (displayingGroup < Files::prefixes.size()) {
                                                                Files::recordGroup(records, skins, gui,
                                                                                   displayingGroup);
                                                                displayingGroup++;
                                                                if (displayingGroup != records.size()) {
                                                                    Files::displayGroup(records, gui, displayingGroup);
                                                                }
                                                            }
                                                            Files::applyGroups(records, true, skins);

                                                            window.close();

                                                        } else if (button.getString() == "Reset") {
                                                            window.close();

                                                        } else if (button.getString() == "Revert last change") {
                                                            Files::revert();
                                                            window.close();
                                                        }

                                                        break;
                                                    }
                                                } catch (std::exception &e) {
                                                    fmt::println("exception in menu handling, button {}, {}",
                                                                 static_cast<std::string>(button.getString()),
                                                                 e.what());
                                                }
                                            }
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
                                        if (lastSel != -1) {
                                            buf = first;
                                            if (first < lastSel) {
                                                while (first < lastSel) {
                                                    if (lastSelected) {
                                                        gui[first].setFillColor(sf::Color::Magenta);
                                                    } else {
                                                        gui[first].setFillColor(sf::Color::White);
                                                    }
                                                    first++;
                                                }
                                            } else {
                                                while (first > lastSel) {
                                                    if (lastSelected) {
                                                        gui[first].setFillColor(sf::Color::Magenta);
                                                    } else {
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

                    } catch (std::exception &e) {
                        fmt::println("exception in event polling, {}", e.what());
                    }

                    window.display();
                }
            }
        }
    }

}
