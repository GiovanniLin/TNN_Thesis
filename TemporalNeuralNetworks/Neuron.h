#pragma once
#include <vector>
#include <numeric>
#include <iostream>
#include <memory>
#include "Connection.h"
#include "IntegrateFire.h"

// Turn inputs into seperate object later so that the input, weight and RIF are paired
class Neuron 
{
public:
	Neuron(int numInputs, int threshold, int ifType);
	std::vector<std::shared_ptr<Connection>> inputs; // Each input and its corresponding weight
	int threshold; // Synaptic threshold
	std::shared_ptr<IntegrateFire> ifType;
	bool output; // Output
	void setIFThreshold(int ifThreshold);
	void overwriteInput(int index, int weight, bool* input);
	void integrateFire(int input);
	void checkThreshold();
	int currentBodyPotential();
	void checkForSpike();
	void checkForIF();
	void removeOutputSpike();
};

