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
                xPlayer = xHos = i;
                yPlayer = yHos = (j + 1) / 2;
                break;
            case State::PORTAL:
                portalLinker(cell.getPortal(), i, (j + 1) / 2);
                break;
            }
        }
    }
}

void Map::setPos(int x, int y)
{   
    currentString = L"";
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
            portalIndex += 2;

            if (portalIndex > portals.size() / 2)
            {
                portalIndex = 0;
            }

            xPlayer = portals[portalIndex];
            yPlayer = portals[portalIndex + 1];

            currentString += L", ��� ��������������� ������";
            break;
        }

        if (map[yPlayer][xPlayer] == State::RIVER_END)
        {
            currentString += L", �� � ����� ����";
        }
    }
}

std::wstring Map::getString()
{
    return currentString;
}

int Map::getPosX()
{
    return xPlayer;
}

int Map::getPosY()
{
    return yPlayer;
}

void Map::portalLinker(int port, int x, int y)
{
    
}


