#include "Cell.h"

Cell::Cell(char type1, char type2)
{
	switch (type1)
	{
	case ' ':
		type = State::SPACE;
		break;
	case 'x':
		type = State::WALL;
		break;
	case '[':
		type = State::WALL;
		break;
	case 'H':
		type = State::HOSPITAL;
		break;
	case 'M':
		type = State::MINOTAUR;
		break;
	case 'D':
		type = State::EXIT;
		break;
	case 'K':
		type = State::KEY_UP;
		break;
	case 'R':
		switch (type2)
		{
		case 'u':
			type = State::RIVER_UP;
			break;
		case 'r':
			type = State::RIVER_RIGHT;
			break;
		case 'd':
			type = State::RIVER_DOWN;
			break;
		case 'l':
			type = State::RIVER_LEFT;
			break;
		case 'e':
			type = State::RIVER_END;
			break;
		}
		break;
	default:
		if (type1 >= 'a' && type1 <= 'z')
		{
			type = State::PORTAL;
			
			portalLink = type1 + type2;
		}
		break;
	}
}

State Cell::getType()
{
	return type;
}

int Cell::getPortal()
{
	return portalLink;
}
