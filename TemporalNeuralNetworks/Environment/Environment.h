#pragma once
#include <iomanip>
#include <iostream>
#include <fstream>
#include <limits>
#include <chrono>
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
	void recordAvgTime(const std::chrono::milliseconds time) { avgTime.push_back(time); }

	bool getTrainingMode() const { return trainingMode; }
	int getTrainingEpisodes() const { return trainingEpisodes; }
	int getTestEpisodes() const { return testEpisodes; }
	bool useRandomAngle() const { return randomAngle; }
	int getCycleLimit() const { return cycleLimit; }
	int getEpisodeResultsDump() const { return episodeResultDump; }
	const std::vector<double>& getAvgCycles() const { return avgCycles; }
	const std::vector<std::chrono::milliseconds>& getAvgTime() const { return avgTime; }
private:
	State state;

	int simCycles = 10;
	bool trainingMode = true;
	int trainingEpisodes = 20;
	int addRewardAfter = 0;
	int testEpisodes = 50;
	bool randomAngle = true;
	int rewardCycleThreshold = 4000;
	int cycleLimit = 80000;
	int episodeResultDump = 0;
	std::vector<double> avgCycles;
	std::vector<std::chrono::milliseconds> avgTime;
};

