#pragma once
#include <iomanip>
#include <iostream>
#include <fstream>
#include <limits>
#include "State.h"

class Environment
{
public:
	void testPrint();
	State getState();
	void resetState(bool random);
	bool decode(int output);
	void printState();
	void writeState(std::ofstream& myfile, int cycle);
	int determineReward(int cycleCounter, int episodeCounter);
	bool stepState(bool action);
	void testMath();
	void recordAvgCycles(const double cycles) { avgCycles.push_back(cycles); }

	bool getTrainingMode() const { return trainingMode; }
	int getTrainingEpisodes() const { return trainingEpisodes; }
	int getTestEpisodes() const { return testEpisodes; }
	bool useRandomAngle() const { return randomAngle; }
	int getCycleLimit() const { return cycleLimit; }
	const std::vector<double>& getAvgCycles() const { return avgCycles; }
private:
	State state;

	int simCycles = 10;
	bool trainingMode = true;
	int trainingEpisodes = 1;
	int addRewardAfter = 1;
	int testEpisodes = 50;
	bool randomAngle = true;
	int rewardCycleThreshold = 500;
	int cycleLimit = 10000;
	std::vector<double> avgCycles;

};

