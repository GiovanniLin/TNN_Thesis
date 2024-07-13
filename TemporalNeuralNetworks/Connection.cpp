#include "Connection.h"

// Default Constructor
// DO NOT USE
Connection::Connection() : spike(nullptr)
{
	this->weight = 0;
	this->dynamicAlloc = false;
}

// Constructor for Connection without input
Connection::Connection(double weight) : spike(nullptr)
{
	this->weight = weight;
	this->dynamicAlloc = false;
}

// Connection: synapse (synaptic crossing) between system input or axon and dendrite
// Axon (output of a neuron) can have synapses (connection areas) to multiple dendrites (inputs of neurons)
Connection::Connection(bool* input, double weight) : spike(input)
{
	this->weight = weight;
	this->dynamicAlloc = false;
}

Connection::~Connection() {
	if (dynamicAlloc && spike != nullptr) {
		delete spike;
	}
}

bool Connection::getSpike()
{
	if (spike != nullptr) {
		return *spike;
	}
	std::cout << "Spike is nullptr \n";
	return false;
}

void Connection::setSpike(bool* spike)
{
	this->spike = spike;
}

double Connection::getWeight()
{
	return weight;
}

int Connection::getWeightRounded()
{
	return int (weight + 0.5);
}

void Connection::setWeight(double weight)
{
	this->weight = weight;
}

void Connection::setDynamicAlloc(bool dynamicAlloc)
{
	this->dynamicAlloc = dynamicAlloc;
}

void Connection::updateWeightCTNN(STDPConfigurator& config, int operation)
{
	if (operation == 0) { // Capture
		double update = weight + config.getCapture();
		update = std::min(std::max(0.0, update), config.getWMaxCTNN());
		setWeight(update);
	}
	else if (operation == 1) { // Backoff
		double update = weight - config.getBackoff();
		update = std::min(std::max(0.0, update), config.getWMaxCTNN());
		setWeight(update);
	}
	else if (operation == 2) { // Search
		double update = weight + config.getSearch();
		update = std::min(std::max(0.0, update), config.getWMaxCTNN());
		setWeight(update);
	}
	else if (operation == 3) { // No-op
		// Do nothing
		return;
	}
	else {
		throw std::runtime_error("Weight update error, invalid weight update operation for C-TNN.");
	}
}

void Connection::updateWeightRTNN(STDPConfigurator& config, int operation, int decayCounter)
{
	if (operation == 0) { // Reward Potentiation
		double count = std::min(decayCounter, config.getRewardW());
		double rewardP = config.getRewardP() - (count * config.getRewardP() / config.getRewardW());
		double update = weight + rewardP;
		update = std::min(std::max(0.0, update), config.getWMaxRTNN());
		setWeight(update);
	}
	else if (operation == 1) { // Reward Depression
		double count = std::min(decayCounter, config.getRewardW());
		double rewardD = config.getRewardD() - (count * config.getRewardD() / config.getRewardW());
		double update = weight - rewardD;
		update = std::min(std::max(0.0, update), config.getWMaxRTNN());
		setWeight(update);
	}
	else if (operation == 2) { // Punishment Potentiation
		double count = std::min(decayCounter, config.getPunishmentW());
		double punishmentP = config.getPunishmentP() - (count * config.getPunishmentP() / config.getPunishmentW());
		double update = weight + punishmentP;
		update = std::min(std::max(0.0, update), config.getWMaxRTNN());
		setWeight(update);
	}
	else if (operation == 3) { // Punishment Depression
		double count = std::min(decayCounter, config.getPunishmentW());
		double punishmentD = config.getPunishmentD() - (count * config.getPunishmentD() / config.getPunishmentW());
		double update = weight - punishmentD;
		update = std::min(std::max(0.0, update), config.getWMaxRTNN());
		setWeight(update);
	}
}