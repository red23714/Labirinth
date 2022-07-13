#pragma once

#include <vector>
#include <iostream>

#include "Cell.h"

class Map
{
public:
	Map(int, int, std::vector<std::string>);

	void setPos(int, int);

	int getPosX();
	int getPosY();
	
	std::wstring getString();
private:
	int xPlayer, yPlayer;

	int xHos, yHos;

	bool inRivPor = false;

	std::vector<std::vector<State>> map;

	std::wstring currentString;

	std::vector<int> portals = {4, 3, 5, 1};
	int portalIndex = 0;

	void portalLinker(int, int, int);
};

