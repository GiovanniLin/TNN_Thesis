#pragma once
#include <memory>
#include <iostream>
#include "STDPConfigurator.h"

class Connection
{
public:
	Connection(); // Default constructor for making arrays and vectors
	Connection(double weight); // If not default input, use this.
	Connection(bool* input, double weight);
	bool* spike; // Always false when constructing
	bool dynamicAlloc;
	bool getSpike();
	void setSpike(bool* spike);
	double getWeight();
	int getWeightRounded();
	void setWeight(double weight);
	void setDynamicAlloc(bool dynamicAlloc);
	void updateWeightCTNN(STDPConfigurator& config, int operation);
	void updateWeightRTNN(STDPConfigurator& config, int operation, int decayCounter);
	~Connection();
private:
	bool defaultSpike = false;
	double weight;
};

