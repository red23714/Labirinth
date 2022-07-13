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

    sf::Text message;
    message.setFont(font);
    message.setCharacterSize(40);
    message.setPosition(0.f, 0.f);
    message.setFillColor(sf::Color::White);

    std::string line;
    std::vector<std::string> lines;
    int sizeX, sizeY;

    Player player;

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

    sf::RenderWindow window(sf::VideoMode(1000, 1000), "Labirinth");

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();

            if (event.type == sf::Event::KeyPressed)
            {
                switch (event.key.code)
                {
                case sf::Keyboard::W:
                    map.setPos(0, -1);
                    break;
                case sf::Keyboard::D:
                    map.setPos(1, 0);
                    break;
                case sf::Keyboard::S:
                    map.setPos(0, 1);
                    break;
                case sf::Keyboard::A:
                    map.setPos(-1, 0);
                    break;
                case sf::Keyboard::Up:
                    map.setPos(0, -1);
                    break;
                case sf::Keyboard::Right:
                    map.setPos(1, 0);
                    break;
                case sf::Keyboard::Down:
                    map.setPos(0, 1);
                    break;
                case sf::Keyboard::Left:
                    map.setPos(-1, 0);
                    break;
                }
                message.setString(map.getString());
                std::cout << map.getPosX() << ' ' << map.getPosY() << '\n';
            }
        }

        window.clear();
        window.draw(message);
        window.display();
    }

    return 0;
}