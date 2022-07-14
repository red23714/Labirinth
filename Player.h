#pragma once

#include <iostream>

#include "State.h"
#include "Cell.h"

class Player
{
public:
	Player();
	Player(int, int);

	void setState(State state);

	bool checkKey();

	int getPosX();
	int getPosY();

	void setPos(int, int);

private:
	bool isKey = false;
	bool isWin = false;

	int xPos = 0;
	int yPos = 0;

	void dead();
	void dropKey();
	void catchedKey();
};

