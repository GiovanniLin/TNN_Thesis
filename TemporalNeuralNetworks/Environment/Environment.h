#pragma once
#include <iostream>
#include "State.h"

class Environment
{
public:
	static void testPrint();
	State getState();
private:
	State state;
};

