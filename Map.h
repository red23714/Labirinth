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
	std::vector<Player*> players;
	Map(int, int, std::vector<std::string>);

	void setPos(int, int);

	State getState(int, int);

	int getHosX();
	int getHosY();

	void setCurrentPlayer(Player*);
	void setPlayers(std::vector<Player> *);

	void spear(int);

	void skip();
	void dead();

	void knife();

	bool getWin();
	
	std::wstring getString();
private:
	Player *currentPlayer;

	int xHos, yHos;

	bool isWin = false;

	std::vector<std::vector<State>> map;

	std::wstring currentString;

	int portIndex = 0;

	std::vector<Portal> portals = {};

	void portalLinker(int, int, int);
};

