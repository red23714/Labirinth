#include "Player.h"

Player::Player()
{

}

Player::Player(int xBegin, int yBegin)
	:
	xPos(xBegin),
	yPos(yBegin)
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
		dead();
		break;
	case State::KEY_UP:
		catchedKey();
		break;
	}
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


