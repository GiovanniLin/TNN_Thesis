#pragma once
#include <iostream>
#include <cmath>
#include "State.h"

class Environment
{
public:
	void testPrint();
	State getState();
	bool decode(int output);
	void printState();
	int determineReward(int cycleCounter);
	bool stepState(bool action);
	void testMath();
	int simCycles = 10;
private:
	State state;
};

