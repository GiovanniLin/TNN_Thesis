#include "Neuron.h"

Neuron::Neuron(int numInputs, int threshold, int ifType) {
	std::vector<std::shared_ptr<Connection>> inputs(numInputs);
	this->inputs = inputs;
	this->threshold = threshold; // Threshold potential of the neuron
	this->ifType = IntegrateFire::chooseIntegrateFire(ifType, numInputs);
	this->output = false;
}

void Neuron::setIFThreshold(int ifThreshold)
{
	this->ifType->setIFThreshold(ifThreshold);
}

void Neuron::overwriteInput(int index, int weight, bool* input)
{
	this->inputs[index] = std::make_shared<Connection>(input, weight);
}

void Neuron::integrateFire(int input)
{
	this->ifType->accumulatePotential(input, this->inputs[input]->weight);
}

void Neuron::checkThreshold()
{
	bool res = this->ifType->checkThreshold(this->threshold);
	if (res) {
		this->output = true;
	}
}

int Neuron::currentBodyPotential()
{
	int res = this->ifType->currentBodyPotential();
	return res;
}

void Neuron::checkForSpike()
{
	for (size_t i = 0; i < this->inputs.size(); i++) {
		if (*(this->inputs[i]->spike)) {
			//std::cout << "Spike detected on input: " << i << " \n";
			this->ifType->setSpikeFlag(i);
			if (inputs[i]->weight >= this->ifType->getIFThreshold() && this->ifType->getIFThreshold() > 0) {
				this->integrateFire(i);
			}
		}
	}
}

void Neuron::checkForIF()
{
	for (size_t i = 0; i < this->inputs.size(); i++) {
		if (this->ifType->getSpikeFlag(i)) {
			this->integrateFire(i);
		}
	}
}

void Neuron::removeOutputSpike()
{
	this->output = false;
}