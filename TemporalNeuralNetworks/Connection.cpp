#include "Connection.h"

// Default Constructor
// DO NOT USE
Connection::Connection() : spike(nullptr)
{
	this->weight = 0;
	this->dynamicAlloc = false;
}

// Constructor for Connection without input
Connection::Connection(int weight) : spike(nullptr)
{
	this->weight = weight;
	this->dynamicAlloc = false;
}

// Connection: axon-to-synapse-to-dendrite-to-neuron body
// Axon (output of a neuron) can have synapses (connection areas) to multiple dendrites (inputs of neurons)
Connection::Connection(bool* input, int weight) : spike(input)
{
	this->weight = weight;
	this->dynamicAlloc = false;
}

Connection::~Connection() {
	if (dynamicAlloc && spike != nullptr) {
		delete spike;
	}
}

void Connection::setSpike(bool* spike)
{
	this->spike = spike;
}

bool Connection::getSpike()
{
	if (spike != nullptr) {
		return *spike;
	}
	std::cout << "Spike is nullptr \n";
	return false;
}

void Connection::setDynamicAlloc(bool dynamicAlloc) {
	this->dynamicAlloc = dynamicAlloc;
}
