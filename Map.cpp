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

void Map::setPlayers(std::vector<Player> *ps)
{
    for (int i = 0; i < (*ps).size(); i++)
    {
        players.push_back(&(*ps)[i]);
    }
}

void Map::setCurrentPlayer(Player *player)
{
    currentPlayer = player;
}

void Map::spear(int dir)
{
    int xSpear = currentPlayer->getPosX();
    int ySpear = currentPlayer->getPosY();

    currentPlayer->spearCount--;

    currentPlayer->setString(L"Вы кинули копьё");

    while (map[ySpear][xSpear] != State::WALL && map[ySpear][xSpear] != State::EXIT)
    {
        if (map[ySpear][xSpear] == State::MINOTAUR)
        {
            map[ySpear][xSpear] = State::MINOTAUR_DEAD;
            break;
        }
        if (map[ySpear][xSpear] == State::MINOTAUR_KEY)
        {
            map[ySpear][xSpear] = State::MINOTAUR_DEAD_KEY;
            break;
        }

        for (int i = 0; i < players.size(); i++)
        {
            if (xSpear == players[i]->getPosX() && ySpear == players[i]->getPosY()
                && players[i]->name != currentPlayer->name)
            {
                players[i]->setState(State::MINOTAUR);

                players[i]->setPos(xHos, yHos);

                if (players[i]->inRivPor) players[i]->inRivPor = false;

                return;
            }
        }

        switch (dir)
        {
        case 1:
            ySpear--;
            break;
        case 2:
            xSpear++;
            break;
        case 3:
            ySpear++;
            break;
        case 4:
            xSpear--;
            break;
        }
        std::cout << currentPlayer->spearCount << ' ' << xSpear << ' ' << ySpear << '\n';
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

        if (currentPlayer->inRivPor) currentPlayer->inRivPor = false;

        currentString = L"Вы прошли дальше";
        break;
    case State::HOSPITAL:
        xPlayer += x;
        yPlayer += y;

        if (currentPlayer->inRivPor) currentPlayer->inRivPor = false;

        currentString = L"Вы прошли дальше и оказались в больнице";
        break;
    case State::KEY_UP:
        xPlayer += x;
        yPlayer += y;

        currentPlayer->setState(State::KEY_UP);

        map[yPlayer][xPlayer] = State::SPACE;

        if (currentPlayer->inRivPor) currentPlayer->inRivPor = false;

        currentString = L"Вы нашли ключ";
        break;
    case State::MINOTAUR:
    case State::MINOTAUR_KEY:

        if (currentPlayer->checkKey())
        {
            map[yPlayer + y][xPlayer + x] = State::MINOTAUR_KEY;
        }

        currentPlayer->setState(State::MINOTAUR);

        xPlayer = xHos;
        yPlayer = yHos;

        if (currentPlayer->inRivPor) currentPlayer->inRivPor = false;

        currentString = L"Вы умерли и оказались в больнице";

        break;
    case State::MINOTAUR_DEAD:
        xPlayer += x;
        yPlayer += y;

        if (currentPlayer->inRivPor) currentPlayer->inRivPor = false;

        currentString = L"Вы нашли труп минотавра";
        break;
    case State::MINOTAUR_DEAD_KEY:
        xPlayer += x;
        yPlayer += y;

        currentPlayer->setState(State::KEY_UP);

        map[yPlayer][xPlayer] = State::MINOTAUR_DEAD;

        if (currentPlayer->inRivPor) currentPlayer->inRivPor = false;

        currentString = L"Вы нашли труп минотавра и ключ";
        break;
    case State::EXIT:
        if (currentPlayer->checkKey())
        {
            currentString = L"Вы выиграли";
            isWin = true;
        }
        else
        {
            currentString = L"Вы нашли выход";
        }
        break;
    case State::RIVER_END:
        xPlayer += x;
        yPlayer += y;

        currentPlayer->inRivPor = false;

        currentString = L"Вы в конце реки";
        break;
    case State::PORTAL:
        xPlayer += x;
        yPlayer += y;

        currentPlayer->inRivPor = true;

        currentString = L"Вы попали в портал";
        break;
    case State::RIVER_UP:
    case State::RIVER_RIGHT:
    case State::RIVER_DOWN:
    case State::RIVER_LEFT:
        xPlayer += x;
        yPlayer += y;

        currentString = L"Вы в реке";
        currentPlayer->inRivPor = true;
        break;
    }

    if (currentPlayer->inRivPor)
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
    currentPlayer->setString(currentString);
}

State Map::getState(int x, int y)
{
    return map[x][y];
}

std::wstring Map::getString()
{
    return currentPlayer->getString();
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

void Map::knife()
{
    int xPlayer = currentPlayer->getPosX();
    int yPlayer = currentPlayer->getPosY();

    for (int i = 0; i < players.size(); i++)
    {
        if (xPlayer == players[i]->getPosX() && yPlayer == players[i]->getPosY() 
            && players[i]->name != currentPlayer->name)
        {
            if (players[i]->checkKey())
            {
                map[yPlayer][xPlayer] = State::KEY_UP;
            }

            players[i]->setState(State::MINOTAUR);

            players[i]->setPos(xHos, yHos);

            if (players[i]->inRivPor) players[i]->inRivPor = false;
        }
    }

    currentPlayer->setString(L"Вы пырнули ножом");
}

void Map::skip()
{
    currentString = L"Вы пропускаете ход";

    if (currentPlayer->inRivPor)
    {
        int xPlayer = currentPlayer->getPosX();
        int yPlayer = currentPlayer->getPosY();
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

        currentPlayer->setPos(xPlayer, yPlayer);
        currentPlayer->setString(currentString);
    }
}

void Map::dead()
{
    int xPlayer = currentPlayer->getPosX();
    int yPlayer = currentPlayer->getPosY();

    if (currentPlayer->checkKey())
    {
        map[yPlayer][xPlayer] = State::KEY_UP;
    }

    currentPlayer->setState(State::MINOTAUR);

    xPlayer = xHos;
    yPlayer = yHos;

    currentPlayer->setPos(xPlayer, yPlayer);

    currentPlayer->setString(L"Вы совершили самоубийство и оказались в больнице");
}

bool Map::getWin()
{
    return isWin;
}
