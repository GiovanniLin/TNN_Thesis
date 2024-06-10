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
	void checkNeuronIFs();
	void checkNeuronSpikes();
	void checkNeuronThresholds();
	std::vector<int> checkOutputs();
	void removeOutputSpikes();
private:
	bool wta =  false;
	int highestBodyPotential = -1;
	int highestBodyPotentialIndex = -1;
};

