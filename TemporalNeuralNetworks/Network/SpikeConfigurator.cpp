#include "SpikeConfigurator.h"

SpikeConfigurator::SpikeConfigurator(std::string spikeConfig) : spikeConfig_(FileReader::FileReader(spikeConfig))
{
	//First line of the spike_config.txt must be "Spike Config:"
	std::string firstLine = spikeConfig_.readNextLine();
	if (firstLine.empty() || firstLine != "Spike Config:") {
		throw std::runtime_error("Spike configuration failed, file passed to SpikeConfigurator is not a spike configuration. Make sure the first line of the text file says 'Spike Config:'");
	}
}

std::vector<std::tuple<int, int>> SpikeConfigurator::createSpikes()
{
	std::vector<std::tuple<int, int>> res;

	while (spikeConfig_.isNextLine()) {
		std::string nextLine = spikeConfig_.readNextLine();
		if (nextLine.empty()) {
			continue;
		}
		else if (nextLine == "Spikes:") {
			while (spikeConfig_.isNextLine()) {
				std::vector<std::string> nL = spikeConfig_.readNextLineSplit(" ");
				if (!nL.empty()) {
					std::tuple<int, int> mapping = spikeHandler(nL);
					if (std::get<0>(mapping) == -1 || std::get<1>(mapping) == -1) {
						if (nL[0] == "End") {
							break;
						}
						else {
							continue;
						}
					}
					else {
						res.push_back(mapping);
					}
				}
			}
			break;
		}
		else {
			throw std::runtime_error("Spike configuration failed, formatting of 'spike_config.txt' is incorrect. File should say 'Spikes:' first and then list all the spikes.");
		}
	}

	if (res.empty()) {
		throw std::runtime_error("Spike configuration failed, no spikes found in 'spike_config.txt'.");
	}

	return res;
}

std::tuple<int, int> SpikeConfigurator::spikeHandler(std::vector<std::string> v)
{
	int spikeInput = -1;
	int spikeTime = -1;

	if (v[0] == "End") {
		return std::make_tuple(spikeInput, spikeTime);
	}

	if (v[0] != "Input:") {
		throw std::runtime_error("Spike configuration failed, invalid formatting of spike, 'Input:' not found. Correct formatting is 'Input: #, Time: #'");
	}
	spikeInput = std::stoi(v[1]);

	if (v[2] != "Time:") {
		throw std::runtime_error("Spike configuration failed, invalid formatting of spike, 'Time:' not found. Correct formatting is 'Input: #, Time: #'");
	}
	spikeTime = std::stoi(v[3]);

	return std::make_tuple(spikeInput, spikeTime);
}