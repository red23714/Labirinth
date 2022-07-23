#pragma once

//#include <iostream>

#include "State.h"
#include "Cell.h"

class Player
{
public:
	Player();
	Player(int, int, std::wstring);

	void setState(State state);

	void setString(std::wstring);
	std::wstring getString();

	bool checkKey();

	int getPosX();
	int getPosY();

	void setPos(int, int);

	std::wstring name = L"";

	bool inRivPor = false;

	int spearCount = 3;

private:
	std::wstring currentString = L"";

	bool isKey = false;
	bool isWin = false;

	int xPos = 0;
	int yPos = 0;

	void dead();
	void dropKey();
	void catchedKey();
};

