#pragma once
#include <vector>
#include <numeric>
#include <iostream>
#include "Neuron.h"
#include "STDPConfigurator.h"

class Layer
{
public:
	std::vector<Neuron> neurons;
	void addNeuron(Neuron neuron);
	void setNeurons(std::vector<Neuron> v);
	bool getWTA();
	void setWTA(bool b);
	int getTypeTNN();
	void setTypeTNN(int typeTNN);
	std::vector<int> getInputTime();
	std::vector<int> getOutputTime();
	std::vector<std::vector<int>> getDecayCounters();
	void checkNeuronIFs();
	void checkNeuronSpikes(int time);
	void checkNeuronThresholds(int time);
	std::vector<int> checkOutputs();
	void removeOutputSpikes();
	void updateWeights(STDPConfigurator& config, int reward);
	void initializeVectors(int x, int y);
	void incrementCounters();
	void setInputTime(int index, int value);
	void setOutputTime(int index, int value);
	void resetCounters(int index);
	int identifyWeightUpdateCTNN(int x, int z);
	int identifyWeightUpdateRTNN(int r, int x, int z);
	void resetNeurons();
private:
	bool wta =  false;
	int highestBodyPotential = -1;
	int highestBodyPotentialIndex = -1;
	int typeTNN = -1; // -1 is not set, 0 is C-TNN, 1 is R-TNN
	std::vector<std::vector<int>> decayCounters;
	std::vector<int> inputTime;
	std::vector<int> outputTime;
};

