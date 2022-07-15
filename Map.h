#pragma once

#include <vector>
#include <iostream>
#include <algorithm>

#include "Cell.h"
#include "Player.h"
#include "Portal.h"

class Map
{
public:
	Map(int, int, std::vector<std::string>);

	void setPos(int, int);

	int getHosX();
	int getHosY();

	void setCurrentPlayer(Player*);

	void setKill(int, int);

	bool getWin();
	
	std::wstring getString();
private:
	Player *currentPlayer;

	int xHos, yHos;

	bool inRivPor = false;

	bool isWin = false;

	std::vector<std::vector<State>> map;

	std::wstring currentString;

	int portIndex = 0;

	std::vector<Portal> portals = {};

	void portalLinker(int, int, int);
};

