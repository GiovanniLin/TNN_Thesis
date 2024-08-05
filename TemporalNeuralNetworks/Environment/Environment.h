#pragma once
#include <iomanip>
#include <iostream>
#include <limits>
#include <cmath>
#include "State.h"

class Environment
{
public:
	void testPrint();
	State getState();
	void resetState(bool random);
	bool decode(int output);
	void printState();
	int determineReward(int cycleCounter, int episodeCounter);
	bool stepState(bool action);
	void testMath();
	int simCycles = 10;
	bool trainingMode = true;
	int trainingEpisodes = 1;
	int addRewardAfter = 1;
	int testEpisodes = 50;
	bool randomAngle = true;
	int rewardCycleThreshold = 500;
	int cycleLimit = 10000;
	std::vector<double> avgCycles;
private:
	State state;
};

