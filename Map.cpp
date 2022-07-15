#include "Map.h"

Map::Map(int sizeX, int sizeY, std::vector<std::string> lines)
{
    map.resize(sizeY);

    for (int i = 0; i < sizeY; i++)
    {
        for (int j = 0; j < sizeX; j+=2)
        {
            Cell cell(lines[i][j], lines[i][j+1]);

            map[i].push_back(cell.getType());

            switch(cell.getType())
            {
            case State::HOSPITAL:
                xHos = i;
                yHos = (j + 1) / 2;
                break;
            case State::PORTAL:
                portalLinker(cell.getPortal(), (j + 1) / 2, i);
                break;
            }
        }
    }

    std::sort(portals.begin(), portals.end(), [](Portal& a, Portal& b) {
        return a.index < b.index; });
}

void Map::setCurrentPlayer(Player *player)
{
    currentPlayer = player;
}

void Map::setKill(int x, int y)
{
    int xPlayer = currentPlayer->getPosX();
    int yPlayer = currentPlayer->getPosY();

    switch (map[yPlayer + y][xPlayer + x])
    {
    case State::MINOTAUR:
        std::cout << 0 << '\n';
        map[yPlayer + y][xPlayer + x] = State::MINOTAUR_DEAD;
        break;
    case State::MINOTAUR_KEY:
        std::cout << 1 << '\n';
        map[yPlayer + y][xPlayer + x] = State::MINOTAUR_DEAD_KEY;
        break;
    }
}

void Map::setPos(int x, int y)
{   
    currentString = L"";

    int xPlayer = currentPlayer->getPosX();
    int yPlayer = currentPlayer->getPosY();

    switch (map[yPlayer + y][xPlayer + x])
    {
    case State::WALL:
        currentString = L"Вы упёрлись в стену";
        break;
    case State::SPACE:
        xPlayer += x;
        yPlayer += y;

        if (inRivPor) inRivPor = false;

        currentString = L"Вы прошли дальше";
        break;
    case State::HOSPITAL:
        xPlayer += x;
        yPlayer += y;

        if (inRivPor) inRivPor = false;

        currentString = L"Вы прошли дальше";
        break;
    case State::KEY_UP:
        xPlayer += x;
        yPlayer += y;

        currentPlayer->setState(State::KEY_UP);

        map[yPlayer][xPlayer] = State::SPACE;

        if (inRivPor) inRivPor = false;

        currentString = L"Вы нашли ключ";
        break;
    case State::MINOTAUR:
    case State::MINOTAUR_KEY:

        if (currentPlayer->checkKey())
        {
            map[yPlayer + y][xPlayer + x] = State::MINOTAUR_KEY;
            std::cout << xPlayer + x << ' ' << yPlayer + y << '\n';
        }

        currentPlayer->setState(State::MINOTAUR);

        xPlayer = xHos;
        yPlayer = yHos;

        if (inRivPor) inRivPor = false;

        currentString = L"Вы оказались в больнице";

        break;
    case State::MINOTAUR_DEAD:
        xPlayer += x;
        yPlayer += y;

        if (inRivPor) inRivPor = false;

        currentString = L"Вы нашли труп минотавра";
        break;
    case State::MINOTAUR_DEAD_KEY:
        xPlayer += x;
        yPlayer += y;

        currentPlayer->setState(State::KEY_UP);

        map[yPlayer][xPlayer] = State::MINOTAUR_DEAD;

        if (inRivPor) inRivPor = false;

        currentString = L"Вы нашли труп минотавра и ключ";
        break;
    case State::EXIT:
        currentString = L"Вы нашли выход";
        break;
    case State::RIVER_END:
        xPlayer += x;
        yPlayer += y;

        currentString = L"Вы в конце реки";
        break;
    case State::PORTAL:
        xPlayer += x;
        yPlayer += y;

        inRivPor = true;

        currentString = L"Вы попали в портал";
        break;
    case State::RIVER_UP:
    case State::RIVER_RIGHT:
    case State::RIVER_DOWN:
    case State::RIVER_LEFT:
        xPlayer += x;
        yPlayer += y;

        currentString = L"Вы в реке";
        inRivPor = true;
        break;
    }

    if (inRivPor)
    {
        switch (map[yPlayer][xPlayer])
        {
        case State::RIVER_UP:
            yPlayer--;
            currentString += L", вас снесло";
            break;
        case State::RIVER_RIGHT:
            xPlayer++;
            currentString += L", вас снесло";
            break;
        case State::RIVER_DOWN:
            yPlayer++;
            currentString += L", вас снесло";
            break;
        case State::RIVER_LEFT:
            xPlayer--;
            currentString += L", вас снесло";
            break;
        case State::PORTAL:
            for (int i = 0; i < portals.size(); i++)
            {
                if ((i + 1) == portals.size())
                {
                    xPlayer = portals[0].x;
                    yPlayer = portals[0].y;
                    break;
                }
                if (xPlayer == portals[i].x && yPlayer == portals[i].y)
                {
                    xPlayer = portals[i + 1].x;
                    yPlayer = portals[i + 1].y;
                    break;
                }
            }

            currentString += L", вас телепортировало дальше";
            break;
        }

        if (map[yPlayer][xPlayer] == State::RIVER_END && currentString != L"Вы в конце реки")
        {
            currentString += L", вы в конце реки";
        }
    }

    currentPlayer->setPos(xPlayer, yPlayer);
}

std::wstring Map::getString()
{
    return currentString;
}

int Map::getHosX()
{
    return xHos;
}

int Map::getHosY()
{
    return yHos;
}

void Map::portalLinker(int port, int x, int y)
{
    int number = port - 'a' - '1';
    Portal a;

    a.index = number;
    a.x = x;
    a.y = y;

    portals.push_back(a);
}
