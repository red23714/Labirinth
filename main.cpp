#include <SFML/Graphics.hpp>

#include <iostream>
#include <fstream>

#include "State.h"
#include "Player.h"
#include "Map.h"

int main()
{   
    setlocale(LC_ALL, "Russian");

    sf::Font font;
    if (!font.loadFromFile("resources/RobotoCondensed-Bold.ttf"))
        return EXIT_FAILURE;

    const int width = 1000;
    const int height = 1000;

    sf::Text message;
    std::vector<sf::Text> messages;
    message.setFont(font);
    message.setCharacterSize(40);
    message.setPosition(0.f, 0.f);
    message.setFillColor(sf::Color::White);

    bool history = false;
    bool isPush = false;

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

    Player player1(map.getHosX(), map.getHosY());
    Player player2(map.getHosX(), map.getHosY());

    sf::RenderWindow window(sf::VideoMode(width, height), "Labirinth");
    //sf::View viewText(sf::FloatRect(0.f, 0.f, 1000.f, 500.f));
    //sf::View viewPrint(sf::FloatRect(0.f, 550.f, 1000.f, 500.f));

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();

            if (event.type == sf::Event::KeyPressed)
            {
                map.setCurrentPlayer(&player2);
                switch (event.key.code)
                {
                case sf::Keyboard::Escape:
                    return 0;
                    break;
                case sf::Keyboard::W:
                    map.setPos(0, -1);
                    isPush = true;
                    break;
                case sf::Keyboard::D:
                    map.setPos(1, 0);
                    isPush = true;
                    break;
                case sf::Keyboard::S:
                    map.setPos(0, 1);
                    isPush = true;
                    break;
                case sf::Keyboard::A:
                    map.setPos(-1, 0);
                    isPush = true;
                    break;
                case sf::Keyboard::Up:
                    map.setPos(0, -1);
                    isPush = true;
                    break;
                case sf::Keyboard::Right:
                    map.setPos(1, 0);
                    isPush = true;
                    break;
                case sf::Keyboard::Down:
                    map.setPos(0, 1);
                    isPush = true;
                    break;
                case sf::Keyboard::Left:
                    map.setPos(-1, 0);
                    isPush = true;
                    break;
                case sf::Keyboard::H:
                    history = !history;
                    break;
                }
                if (isPush)
                {
                    message.setString(map.getString());
                    messages.push_back(message);
                    std::cout << "Player1: " << player1.getPosX() << ' ' << player1.getPosY() << '\n';
                    std::cout << "Player2: " << player2.getPosX() << ' ' << player2.getPosY() << '\n';
                    isPush = false;
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