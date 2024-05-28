#pragma once
#include <memory>
#include <iostream>

class Connection
{
public:
	Connection(); // Default constructor for making arrays and vectors
	Connection(int weight); // If not default input, use this.
	Connection(bool* input, int weight);
	bool* spike; // Always false when constructing
	int weight; // Only for the neuron having this connection has input
	bool dynamicAlloc;
	void setSpike(bool* spike);
	bool getSpike();
	void setDynamicAlloc(bool dynamicAlloc);
	~Connection();
private:
	bool defaultSpike = false;
};

