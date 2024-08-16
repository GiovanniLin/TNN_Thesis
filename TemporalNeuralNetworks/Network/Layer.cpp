#include "Layer.h"

void Layer::addNeuron(Neuron neuron)
{
	neurons.push_back(neuron);
}

void Layer::setNeurons(std::vector<Neuron> v)
{
	neurons = v;
}

bool Layer::getWTA()
{
	return wta;
}

void Layer::setWTA(bool b)
{
	wta = b;
}

int Layer::getTypeTNN()
{
	return typeTNN;
}

void Layer::setTypeTNN(int typeTNN)
{
	this->typeTNN = typeTNN;
}

bool Layer::getReadWeights()
{
	return readWeights;
}

void Layer::setReadWeights(bool readWeights)
{
	this->readWeights = readWeights;
}

std::vector<int> Layer::getInputTime()
{
	return inputTime;
}

std::vector<int> Layer::getOutputTime()
{
	return outputTime;
}

std::vector<std::vector<int>> Layer::getDecayCounters()
{
	return decayCounters;
}

void Layer::checkNeuronIFs()
{
	for (int i = 0; i < neurons.size(); ++i) {
		//std::cout << "Neuron: " << i << ", ";
		neurons[i].checkForIF();
	}
	//std::cout << "\n";
}

void Layer::checkNeuronSpikes(int time)
{
	for (int i = 0; i < neurons.size(); ++i) {
		neurons[i].checkForSpike();
	}
}

void Layer::checkNeuronThresholds(int time)
{
	for (int i = 0; i < neurons.size(); ++i) {
		// Only check thresholds, if no output spike has been generated yet (no winner yet)
		if (!wta) {
			neurons[i].checkThreshold();
		}
	}

	for (int i = 0; i < neurons.size(); ++i) {
		// If an output spike is produced
		if (neurons[i].output) {
			// Set WTA to true, because an output spike has been generated
			if (!wta) {
				wta = true;
			}

			// Code to check for conflicts at same time output spikes
			// Selects highest body potential neuron as winner.
			// If body potential is equal, select lowest index.
			// Check if this output spike is the one with the highestBodyPotential
			if (neurons[i].currentBodyPotential() > highestBodyPotential) {
				// Save the new highestBodyPotential Neuron
				highestBodyPotential = neurons[i].currentBodyPotential();

				// If highestBodyPotential has been set before, remove the spike from the previous highestBodyPotential Neuron
				if (highestBodyPotentialIndex != -1) {
					neurons[highestBodyPotentialIndex].removeOutputSpike();
				}

				// Save the index of the new highestBodyPotential Neuron
				highestBodyPotentialIndex = i;
			}
			// If this neuron is not highestBodyPotential Neuron, remove it's output spike, because it is not the winner.
			else {
				neurons[i].removeOutputSpike();
			}
		}
	}

	for (int i = 0; i < neurons.size(); ++i) {
		if (neurons[i].output) {
			setOutputTime(i, time);
		}
	}

}

std::vector<int> Layer::checkOutputs()
{
	std::vector<int> res;

	for (int i = 0; i < neurons.size(); ++i) {
		if (neurons[i].output) {
			res.push_back(i);
		}
	}

	return res;
}

void Layer::removeOutputSpikes()
{
	for (int i = 0; i < neurons.size(); ++i) {
		neurons[i].removeOutputSpike();
	}
}

void Layer::setWeights(std::vector<std::vector<double>> weights) {
	for (int i = 0; i < neurons.size(); ++i) {
		for (int j = 0; j < neurons[i].inputs.size(); ++j) {
			//std::cout << "Current weight of Neuron " << i << ", Input " << j << ", Weight: " << neurons[i].inputs[j]->getWeight() << "\n";
			//std::cout << "Setting weight for Neuron " << i << ", Input " << j << ", Weight: " << weights[i][j] << "\n\n";
			neurons[i].setWeight(j, weights[i][j]);
		}
	}
}

void Layer::updateWeights(STDPConfigurator& config, int reward)
{
	for (int i = 0; i < neurons.size(); ++i) {
		for (int j = 0; j < neurons[i].inputs.size(); ++j) {
			int operation = -1;
			if (typeTNN == 0) {
				operation = identifyWeightUpdateCTNN(j, i);
				if (operation == -1) {
					throw std::runtime_error("Cannot update weights, invalid C-TNN weight update input condition.");
				}
			}
			else if (typeTNN == 1) {
				if (reward == 0) {
					continue;
				}
				operation = identifyWeightUpdateRTNN(reward, j, i);
				if (operation == -1) {
					throw std::runtime_error("Cannot update weights, invalid R-TNN weight update input condition.");
				}
			}
			else {
				throw std::runtime_error("Cannot update weights, layer TNN type was not set.");
			}

			if (operation == -1) {
				throw std::runtime_error("Cannot update weights, invalid network state.");
			}
			neurons[i].updateWeight(j, typeTNN, config, decayCounters[j][i], operation);
		}
	}
}

void Layer::initializeVectors(int x, int y)
{
	inputTime.clear();
	outputTime.clear();
	decayCounters.clear();
	for (int i = 0; i < x; ++i) {
		std::vector<int> toAddA;
		for (int j = 0; j < y; ++j) {
			toAddA.push_back(-1);
		}
		decayCounters.push_back(toAddA);
		inputTime.push_back(-1);
	}
	for (int i = 0; i < y; ++i) {
		outputTime.push_back(-1);
	}
	incrementCounters();
}

void Layer::initializeVectorsNoDecay(int x, int y)
{
	inputTime.clear();
	outputTime.clear();
	for (int i = 0; i < x; ++i) {
		std::vector<int> toAddA;
		for (int j = 0; j < y; ++j) {
			toAddA.push_back(-1);
		}
		inputTime.push_back(-1);
	}
	for (int i = 0; i < y; ++i) {
		outputTime.push_back(-1);
	}
}

void Layer::incrementCounters()
{
	for (int i = 0; i < decayCounters.size(); ++i) {
		for (int j = 0; j < decayCounters[i].size(); ++j) {
			decayCounters[i][j] += 1;
		}
	}
}

void Layer::setInputTime(int index, int value)
{
	inputTime[index] = value;
}

void Layer::setOutputTime(int index, int value)
{
	outputTime[index] = value;
}

void Layer::resetCounters(int index)
{
	for (int i = 0; i < decayCounters[index].size(); ++i) {
		decayCounters[index][i] = 0;
	}
}

// 0 is Capture, 1 is backoff, 2 is search, 3 is no-op, -1 is error
// x is index of input, z is index of neuron/output
int Layer::identifyWeightUpdateCTNN(int x, int z)
{
	if (inputTime[x] != -1 && outputTime[z] != -1 && inputTime[x] <= outputTime[z]) {
		return 0;
	}
	else if (inputTime[x] != -1 && outputTime[z] != -1 && inputTime[x] > outputTime[z]) {
		return 1;
	}
	else if (inputTime[x] != -1 && outputTime[z] == -1) {
		return 2;
	}
	else if (inputTime[x] == -1 && outputTime[z] != -1) {
		return 1;
	}
	else if (inputTime[x] == -1 && outputTime[z] == -1) {
		return 3;
	}
	else {
		return -1;
	}
}

// 0 is Reward Potentiation, 1 Reward Depression, 2 Punishment Potentiaion, 3 Punishment Depression, -1 is error
// r is reward, x is index of input, z is index of neuron/output
// e is binary flag, true if x == z
int Layer::identifyWeightUpdateRTNN(int r, int x, int z)
{
	bool e = inputTime[x] != -1 && outputTime[z] != -1;
	if (r == 1 && e) {
		return 0;
	}
	else if (r == 1 && !e) {
		return 1;
	}
	else if (r == -1 && !e) {
		return 2;
	}
	else if (r == -1 && e) {
		return 3;
	}
	else {
		return -1;
	}
}

void Layer::resetNeurons()
{
	for (int i = 0; i < neurons.size(); ++i) {
		neurons[i].resetNeuron();
	}
	wta = false;
	highestBodyPotential = -1;
	highestBodyPotentialIndex = -1;
}