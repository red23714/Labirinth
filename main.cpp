#include <SFML/Graphics.hpp>

#include <iostream>
#include <fstream>
#include <string>

#include "State.h"
#include "Player.h"
#include "Map.h"

const int width = 1000;
const int height = 1000;

void menu(sf::RenderWindow &window)
{
    sf::Texture menuTexture1, menuTexture3, menuBackground;
    menuTexture1.loadFromFile("resources/button.png");
    menuTexture3.loadFromFile("resources/button.png");
    menuBackground.loadFromFile("resources/background.png");

    sf::Sprite menu1(menuTexture1), menu3(menuTexture3), menuBg(menuBackground);

    bool isMenu = true;
    int menuNum = 0;

    menu1.setPosition(width / 2, height / 2 + 50);
    menu3.setPosition(width / 2, height / 2);
    menuBg.setPosition(0, 0);

    while (isMenu)
    {
        menu1.setColor(sf::Color(0, 51, 102));
        menu3.setColor(sf::Color(0, 51, 102));

        menuNum = 0;

        window.clear(sf::Color(64, 64, 64));

        if (sf::IntRect(width / 2, height / 2 + 25, 300, 50).contains(sf::Mouse::getPosition(window)))
        {
            menu1.setColor(sf::Color(2, 0, 63));
            menuNum = 1;
        }
        if (sf::IntRect(width / 2, height / 2 + 75, 300, 50).contains(sf::Mouse::getPosition(window)))
        {
            menu3.setColor(sf::Color(2, 0, 63));
            menuNum = 3;
        }

        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
        {
            if (menuNum == 1) isMenu = false;
            if (menuNum == 3)
            {
                window.close();
                isMenu = false;
            }
        }

        window.draw(menuBg);
        window.draw(menu1);
        window.draw(menu3);

        window.display();
    }
}

int main()
{   
    setlocale(LC_ALL, "Russian");

    sf::Font font;
    if (!font.loadFromFile("resources/RobotoCondensed-Bold.ttf"))
        return EXIT_FAILURE;

    sf::Text message;
    std::vector<sf::Text> messages;
    message.setFont(font);
    message.setCharacterSize(40);
    message.setPosition(0.f, 0.f);
    message.setFillColor(sf::Color::White);

    bool history = false;
    bool isPush = false;
    bool isKill = false;

    std::string line;
    std::vector<std::string> lines;
    int sizeX, sizeY;

    std::ifstream in("resources/1.txt");
    
    if (in.is_open())
    {
        while (getline(in, line))
        {
            lines.push_back(line);
        }
    }
    else
    {
        std::cout << "File not opened or empty";
    }

    in.close();

    for (int i = 0; i < lines.size(); i++)
    {
        std::cout << lines[i] << '\n';
    }

    sizeX = lines[0].size();
    sizeY = lines.size();

    Map map(sizeX, sizeY, lines);

    std::vector<Player> players;
    int count = 0;
    std::cin >> count;

    int turn = 0;

    for (int i = 0; i < count; i++)
    {
        Player player(map.getHosX(), map.getHosY());
        players.push_back(player);
    }

    sf::RenderWindow window(sf::VideoMode(width, height), "Labirinth");
    menu(window);

    map.setCurrentPlayer(&players[turn]);

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();

            if (event.type == sf::Event::KeyPressed)
            {
                if (event.key.code == sf::Keyboard::Escape)
                {
                    return 0;
                }
            }

            if (event.type == sf::Event::KeyPressed && !map.getWin())
            {
                if (!isKill)
                {
                    switch (event.key.code)
                    {
                    case sf::Keyboard::W:
                    case sf::Keyboard::Up:
                        map.setPos(0, -1);
                        isPush = true;
                        break;
                    case sf::Keyboard::D:
                    case sf::Keyboard::Right:
                        map.setPos(1, 0);
                        isPush = true;
                        break;
                    case sf::Keyboard::S:
                    case sf::Keyboard::Down:
                        map.setPos(0, 1);
                        isPush = true;
                        break;
                    case sf::Keyboard::A:
                    case sf::Keyboard::Left:
                        map.setPos(-1, 0);
                        isPush = true;
                        break;
                    case sf::Keyboard::K:
                        isKill = true;
                        break;
                    case sf::Keyboard::H:
                        history = !history;
                        break;
                    }
                    if (isPush)
                    {
                        std::wstring turnS = L"Player " + std::to_wstring(turn + 1) + L':';
                        message.setString(turnS + map.getString());
                        messages.push_back(message);
                        std::cout << turn << ' ' << players[turn].getPosX() << ' ' << players[turn].getPosY() << '\n';
                        isPush = false;

                        if (turn != count - 1) turn++;
                        else turn = 0;

                        map.setCurrentPlayer(&players[turn]);
                    }
                }
                else
                {
                    message.setString(L"Вы пырнули ножом");
                    switch (event.key.code)
                    {
                    case sf::Keyboard::Up:
                        map.setKill(0, -1);
                        isKill = false;
                        break;
                    case sf::Keyboard::Right:
                        map.setKill(1, 0);
                        isKill = false;
                        break;
                    case sf::Keyboard::Down:
                        map.setKill(0, 1);
                        isKill = false;
                        break;
                    case sf::Keyboard::Left:
                        map.setKill(-1, 0);
                        isKill = false;
                        break;
                    case sf::Keyboard::K:
                        isKill = false;
                        message.setString(L"Вы убрали нож");
                        break;
                    }
                }
            }
        }

        window.clear();
        if (history)
        {
            for (int i = 0; i < messages.size(); i++)
            {
                int print = i * 40.f;
                if (print >= height)
                {
                    window.clear();
                    break;
                }
                messages[i].setPosition(0.f, print);
                window.draw(messages[i]);
            }
        }
        else
        {
            window.draw(message);
        }
        window.display();
    }

    return 0;
}