#include <SFML/Graphics.hpp>

#include <iostream>
#include <fstream>

#include "State.h"
#include "Player.h"
#include "Map.h"

#define DEBUG 1

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

bool checkName(std::wstring name, std::vector<Player> players)
{
    for (int i = 0; i < players.size(); i++)
    {
        if (name == players[i].name) return true;
    }

    return false;
}

void replay(Map *map, std::wstring pName, std::vector<std::string> lines, sf::RenderWindow &window, sf::Font font)
{
    std::ifstream in(pName);

    std::string line;
    std::vector<std::string> newMap;
    std::vector<int> moves;

    sf::Text text;
    text.setFont(font);
    text.setCharacterSize(30);
    text.setPosition(0.f, 0.f);
    text.setFillColor(sf::Color::White);

    newMap = lines;

    Player player(map->getHosX(), map->getHosY(), L"Test");

    map->setCurrentPlayer(&player);

    if (in.is_open())
    {
        while (getline(in, line))
        {
            moves.push_back(std::stoi(line));
        }
    }
    else
    {
        std::cout << "File not opened or empty";
    }

    for (int i = 0; i < moves.size(); i++)
    {
        if (DEBUG)
        {
            std::cout << moves[i] << '\n';
            std::cout << player.getPosX() << ' ' << player.getPosY() << '\n';
        }

        switch (moves[i])
        {
        case 1:
            map->setPos(0, -1);
            break;
        case 2:
            map->setPos(1, 0);
            break;
        case 3:
            map->setPos(0, 1);
            break;
        case 4:
            map->setPos(-1, 0);
            break;
        case 5:
            map->skip();
            break;
        case 6:
            map->dead();
            break;
        case 7:
            map->knife();
            break;
        case 8:
            map->spear(1);
            break;
        case 9:
            map->spear(2);
            break;
        case 10:
            map->spear(3);
            break;
        case 11:
            map->spear(4);
            break;
        }

        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
            {
                window.close();
                return;
            }
        }

        window.clear();
        int posOfScreen = 0;
        newMap = lines;
        newMap[player.getPosY()][player.getPosX()*2] = 'P';
        newMap[player.getPosY()][(player.getPosX())*2 + 1] = 'P';
        for (int i = 0; i < lines.size(); i++)
        {
            std::cout << newMap[i] << '\n';

            int posOfScreen = i * 30.f;

            text.setPosition(0.f, posOfScreen);
            text.setString(newMap[i]);

            window.draw(text);
        }
        sf::sleep(sf::seconds(1.0));
        window.display();
    }
}

int main()
{   
    setlocale(LC_ALL, "Russian");

    sf::Font font;
    if (!font.loadFromFile("resources/RobotoMono-Light.ttf"))
    {
        return EXIT_FAILURE;
    }

    sf::Text message;
    sf::Text historyMessage;
    std::vector < std::vector<std::wstring> > messages;
    message.setFont(font);
    message.setCharacterSize(25);
    message.setPosition(0.f, 0.f);
    message.setFillColor(sf::Color::White);
    historyMessage.setFont(font);
    historyMessage.setCharacterSize(25);
    historyMessage.setPosition(0.f, 0.f);
    historyMessage.setFillColor(sf::Color::White);

    bool history = false;
    bool isPush = false;
    bool isSpear = false;

    int offset = 0;

    int currentPlayer = 0;

    short whichButt = 0;
    std::wstring currButt = L"";

    std::string line;
    std::vector<std::string> lines;
    int sizeX, sizeY;

    srand(time(NULL));

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

    if (DEBUG)
    {
        for (int i = 0; i < lines.size(); i++)
        {
            std::cout << lines[i] << '\n';
        }
    }

    sizeX = lines[0].size();
    sizeY = lines.size();

    Map map(sizeX, sizeY, lines);

    std::vector<Player> players;
    int count = 0;

    std::cout << "Введите количество игроков: " << '\n';
    std::cin >> count;

    if (count <= 0)
    {
        std::cout << "Пошел нах**";
        return 0;
    }
    else
    {
        messages.resize((int)count);
    }

    int turn = 0;

    for (int i = 0; i < count; i++)
    {
        std::wstring name;
        std::cout << "Введите имя игорька:" << '\n';
        std::wcin >> name;

        while (checkName(name, players))
        {
            std::cout << "Это имя занято" << '\n';
            std::wcin >> name;
        }

        Player player(map.getHosX(), map.getHosY(), name);
        players.push_back(player);
    }

    sf::RenderWindow window(sf::VideoMode(width, height), "Labirinth");
    if (!DEBUG)
    {
        menu(window);
    }

    map.setCurrentPlayer(&players[turn]);
    map.setPlayers(&players);

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
            {
                window.close();
            }

            if (event.type == sf::Event::KeyPressed)
            {
                if (event.key.code == sf::Keyboard::Escape)
                {
                    window.close();
                }
            }

            if (event.type == sf::Event::MouseWheelMoved && history)
            {
                offset -= event.mouseWheel.delta;
            }

            if (event.type == sf::Event::KeyPressed && !map.getWin())
            {
                switch (event.key.code)
                {
                case sf::Keyboard::W:
                case sf::Keyboard::Up:
                    if (isSpear)
                    {
                        map.spear(1);
                        whichButt = 8;
                        currButt = L"Копьё вверх ";
                        isSpear = false;
                    }
                    else
                    {
                        map.setPos(0, -1);
                        whichButt = 1;
                        currButt = L"Вверх ";
                    }
                    isPush = true;
                    history = false;
                    break;
                case sf::Keyboard::D:
                case sf::Keyboard::Right:
                    if (isSpear)
                    {
                        map.spear(2);
                        whichButt = 9;
                        currButt = L"Копьё вправо ";
                        isSpear = false;
                    }
                    else
                    {
                        map.setPos(1, 0);
                        whichButt = 2;
                        currButt = L"Вправо ";
                    }
                    isPush = true;
                    history = false;
                    break;
                case sf::Keyboard::S:
                case sf::Keyboard::Down:
                    if (isSpear)
                    {
                        map.spear(3);
                        whichButt = 10;
                        currButt = L"Копьё вниз ";
                        isSpear = false;
                    }
                    else
                    {
                        map.setPos(0, 1);
                        whichButt = 3;
                        currButt = L"Вниз ";
                    }
                    isPush = true;
                    history = false;
                    break;
                case sf::Keyboard::A:
                case sf::Keyboard::Left:
                    if (isSpear)
                    {
                        map.spear(4);
                        whichButt = 11;
                        currButt = L"Копьё влево ";
                        isSpear = false;
                    }
                    else
                    {
                        map.setPos(-1, 0);
                        whichButt = 4;
                        currButt = L"Влево ";
                    }
                    isPush = true;
                    history = false;
                    break;
                case sf::Keyboard::P:
                    map.skip();
                    whichButt = 5;
                    isPush = true;
                    history = false;
                    currButt = L"Пропуск ";
                    break;
                case sf::Keyboard::F:
                    map.dead();
                    whichButt = 6;
                    isPush = true;
                    history = false;
                    currButt = L"Убился ";
                    break;
                case sf::Keyboard::K:
                    map.knife();
                    whichButt = 7;
                    isPush = true;
                    history = false;
                    currButt = L"Пырнул ";
                    break;
                case sf::Keyboard::C:
                    message.setString(players[turn].name + L" взял копьё");
                    isSpear = !isSpear;
                    history = false;
                    break;
                case sf::Keyboard::H:
                    history = !history;
                    break;
                }

                if (isPush)
                {
                    if (turn >= players.size() - 1) currentPlayer = 0;
                    else currentPlayer = turn + 1;

                    std::wstring currentTurn = L"Ход игрока " + players[currentPlayer].name + L'\n';
                    std::wstring turnS = currButt + players[turn].name + L": ";
                    message.setString(currentTurn + turnS + players[turn].getString());
                    messages[turn].push_back(turnS + map.getString());
                    
                    std::wofstream out;

                    out.open(L"resources/" + players[turn].name + L".txt", std::ios::app); 

                    std::wstring stringTo = std::to_wstring(whichButt);

                    if (out.is_open())
                    {
                        out << stringTo << std::endl;
                    }
                    out.close();

                    if (DEBUG)
                    {
                        std::cout << turn << ' ' << players[turn].getPosX()
                            << ' ' << players[turn].getPosY() << '\n';
                    }
                    isPush = false;

                    if (turn != count - 1) turn++;
                    else turn = 0;

                    map.setCurrentPlayer(&players[turn]);
                }
            }    
        }

        window.clear();
        if (map.getWin())
        {
            for (int i = 0; i < players.size(); i++)
            {
                replay(&map, L"resources/" + players[i].name + L".txt", lines, window, font);
            }
            window.close();
        }
        else
        {
            if (history)
            {
                int historyTurn = turn;

                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) offset--;
                else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) offset++;

                for (int i = 0; i < messages[historyTurn].size(); i++)
                {
                    int print = i * 25.f;
                    print -= offset * 25.f;

                    historyMessage.setString(messages[turn][i]);
                    historyMessage.setPosition(0.f, print);
                    window.draw(historyMessage);
                }
            }
            else
            {
                window.draw(message);
            }
        }
        window.display();
    }

    return 0;
}