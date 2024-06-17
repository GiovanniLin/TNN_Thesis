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

void Layer::checkNeuronIFs()
{
	for (int i = 0; i < neurons.size(); ++i) {
		neurons[i].checkForIF();
	}
}

void Layer::checkNeuronSpikes()
{
	for (int i = 0; i < neurons.size(); ++i) {
		std::cout << "Checking spike on neuron: " << i << " \n";
		neurons[i].checkForSpike();
	}
}

void Layer::checkNeuronThresholds()
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
