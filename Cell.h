#pragma once

#include "State.h"
#include <string>

class Cell
{
public:
	Cell(char, char);

	State getType();

	int getPortal();

private:
	State type;

	int portalLink;
};

