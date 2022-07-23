#include "Player.h"

Player::Player()
{

}

Player::Player(int xBegin, int yBegin, std::wstring name)
	:
	xPos(xBegin),
	yPos(yBegin),
	name(name)
{

}

void Player::catchedKey()
{
	isKey = true;
}

void Player::dropKey()
{
	isKey = false;
}

void Player::dead()
{
	dropKey();
}

void Player::setState(State state)
{
	switch (state)
	{
	case State::MINOTAUR:
		currentString = L"Вы умерли";
		dead();
		break;
	case State::KEY_UP:
		catchedKey();
		break;
	}
}

void Player::setString(std::wstring string)
{
	currentString = string;
}

std::wstring Player::getString()
{
	return currentString;
}

void Player::setPos(int x, int y)
{
	xPos = x;
	yPos = y;
}

int Player::getPosX()
{
	return xPos;
}

int Player::getPosY()
{
	return yPos;
}

bool Player::checkKey()
{
	return isKey;
}


