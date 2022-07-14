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
                /*xPlayer = xHos = i;
                yPlayer = yHos = (j + 1) / 2;*/
                break;
            case State::PORTAL:
                portalLinker(cell.getPortal(), i, (j + 1) / 2);
                break;
            }
        }
    }
}

void Map::setCurrentPlayer(Player *player)
{
    currentPlayer = player;
}

void Map::setPos(int x, int y)
{   
    currentString = L"";

    int xPlayer = currentPlayer->getPosX();
    int yPlayer = currentPlayer->getPosY();

    switch (map[yPlayer + y][xPlayer + x])
    {
    case State::WALL:
        currentString = L"�� ������� � �����";
        break;
    case State::SPACE:
        xPlayer += x;
        yPlayer += y;

        if (inRivPor) inRivPor = false;

        currentString = L"�� ������ ������";
        break;
    case State::HOSPITAL:
        xPlayer += x;
        yPlayer += y;

        if (inRivPor) inRivPor = false;

        currentString = L"�� ������ ������";
        break;
    case State::KEY_UP:
        xPlayer += x;
        yPlayer += y;

        map[yPlayer][xPlayer] = State::SPACE;

        currentString = L"�� ����� ����";
        break;
    case State::MINOTAUR:
        xPlayer = xHos;
        yPlayer = yHos;

        if (inRivPor) inRivPor = false;

        currentString = L"�� ��������� � ��������";
        break;
    case State::EXIT:
        currentString = L"�� ����� �����";
        break;
    case State::RIVER_END:
        xPlayer += x;
        yPlayer += y;

        currentString = L"�� � ����� ����";
        break;
    case State::PORTAL:
        xPlayer += x;
        yPlayer += y;

        inRivPor = true;

        currentString = L"�� ������ � ������";
        break;
    case State::RIVER_UP:
    case State::RIVER_RIGHT:
    case State::RIVER_DOWN:
    case State::RIVER_LEFT:
        xPlayer += x;
        yPlayer += y;

        currentString = L"�� � ����";
        inRivPor = true;
        break;
    }

    if (inRivPor)
    {
        switch (map[yPlayer][xPlayer])
        {
        case State::RIVER_UP:
            yPlayer--;
            currentString += L", ��� ������";
            break;
        case State::RIVER_RIGHT:
            xPlayer++;
            currentString += L", ��� ������";
            break;
        case State::RIVER_DOWN:
            yPlayer++;
            currentString += L", ��� ������";
            break;
        case State::RIVER_LEFT:
            xPlayer--;
            currentString += L", ��� ������";
            break;
        case State::PORTAL:
            currentString += L", ��� ��������������� ������";
            break;
        }

        if (map[yPlayer][xPlayer] == State::RIVER_END && currentString != L"�� � ����� ����")
        {
            currentString += L", �� � ����� ����";
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
    portals.push_back(number);
    portals.push_back(x);
    portals.push_back(y);
}


