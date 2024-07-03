#pragma once
#include <vector>
#include <numeric>
#include <iostream>
#include "Neuron.h"

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
	void checkNeuronIFs();
	void checkNeuronSpikes();
	void checkNeuronThresholds();
	std::vector<int> checkOutputs();
	void removeOutputSpikes();
private:
	bool wta =  false;
	int highestBodyPotential = -1;
	int highestBodyPotentialIndex = -1;
	int typeTNN = -1; // -1 is not set, 0 is C-TNN, 1 is R-TNN
};

