#include "IntegrateFire.h"

// 0 for Ramp Integrate and Fire
// 1 for Leaky Integrate and Fire
// Any other value defaults to Ramp Integrate and Fire, later change this to throwing an error
std::shared_ptr<IntegrateFire> IntegrateFire::chooseIntegrateFire(int type, int numInputs)
{
	if (type == 0) {
		return std::make_shared<RampIntegrateFire>(numInputs);
	}
	else if (type == 1)
	{
		return std::make_shared<LeakyIntegrateFire>();
	}
	else
	{
		throw std::invalid_argument("Parameter 'type' must be 0 or 1");
	}
}

RampIntegrateFire::RampIntegrateFire(int numInputs)
{
	if (numInputs < 1) {
		throw std::invalid_argument("numInputs must be 1 or higher");
	}
	this->rifCounter = std::vector<int>(numInputs, 0);
	this->spikeFlag = std::vector<bool>(numInputs, false);
}

int RampIntegrateFire::getIFThreshold()
{
	return rifThreshold;
}

void RampIntegrateFire::setIFThreshold(int ifThreshold)
{
	this->rifThreshold = ifThreshold;
}

void RampIntegrateFire::accumulatePotential(int input, int weight)
{
	if (this->rifCounter[input] < weight) {
		this->rifCounter[input] += 1;
	}
}

bool RampIntegrateFire::checkThreshold(int threshold)
{
	int bodyPotential = std::accumulate(this->rifCounter.begin(), this->rifCounter.end(), 0);
	if (bodyPotential < threshold) {
		return false;
	}
	else {
		return true;
	}
}

int RampIntegrateFire::currentBodyPotential()
{
	int res = std::accumulate(this->rifCounter.begin(), this->rifCounter.end(), 0);
	return res;
}

void RampIntegrateFire::setSpikeFlag(int index)
{
	this->spikeFlag[index] = true;
}

void RampIntegrateFire::resetAllSpikeFlag()
{
	for (size_t i = 0; i < this->spikeFlag.size(); i++) {
		this->spikeFlag[i] = false;
	}
}

bool RampIntegrateFire::getSpikeFlag(int index)
{
	return this->spikeFlag[index];
}

void RampIntegrateFire::resetPotential()
{
	for (int i = 0; i < rifCounter.size(); ++i) {
		rifCounter[i] = 0;
	}
	resetAllSpikeFlag();
}