#pragma once

#include <vector>
#include <iostream>
#include <algorithm>

#include "Cell.h"
#include "Player.h"

class Map
{
public:
	Map(int, int, std::vector<std::string>);

	void setPos(int, int);

	int getHosX();
	int getHosY();

	void setCurrentPlayer(Player*);
	
	std::wstring getString();
private:
	Player *currentPlayer;

	int xHos, yHos;

	bool inRivPor = false;

	std::vector<std::vector<State>> map;

	std::wstring currentString;

	std::vector<int> portals = {};

	void portalLinker(int, int, int);
};

