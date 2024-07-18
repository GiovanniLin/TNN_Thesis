#include "MappingConfigurator.h"

MappingConfigurator::MappingConfigurator(std::string mappingConfig) : mappingConfig_(FileReader::FileReader(mappingConfig))
{
	//First line of the mapping_config.txt must be "Mapping Config:"
	std::string firstLine = mappingConfig_.readNextLine();
	if (firstLine.empty() || firstLine != "Mapping Config:") {
		throw std::runtime_error("Mapping configuration failed, file passed to MappingConfigurator is not a mapping configuration. Make sure the first line of the text file says 'Mapping Config:'");
	}

	while (mappingConfig_.isNextLine()) {
		std::vector<std::string> nextLine = mappingConfig_.readNextLineSplit(" ");
		if (!nextLine.empty()) {
			configHandler(nextLine);
		}
		if (fullyConnected != -1) {
			break;
		}
	}

	if (fullyConnected == -1) {
		throw std::runtime_error("Mapping configuration failed, 'FullyConnected' not specified");
	}
}

void MappingConfigurator::setFullyConnected(int fullyConnected)
{
	this->fullyConnected = fullyConnected;
}

void MappingConfigurator::configHandler(std::vector<std::string> v)
{
	if (v[0] == "FullyConnected:") {
		if (v[1] == "Yes" || v[1] == "yes") {
			setFullyConnected(1);
		}
		else if (v[1] == "No" || v[1] == "no") {
			setFullyConnected(0);
		}
	}
	else {
		return; // do nothing
	}
}

std::vector<std::tuple<int, int, int>> MappingConfigurator::createInputMap()
{
	std::vector<std::tuple<int, int, int>> res;

	if (fullyConnected == 0) {
		while (mappingConfig_.isNextLine()) {
			std::string nextLine = mappingConfig_.readNextLine();
			if (nextLine.empty()) {
				continue;
			}
			else if (nextLine == "Input Map:") {
				while (mappingConfig_.isNextLine()) {
					std::vector<std::string> nL = mappingConfig_.readNextLineSplit(" ");
					if (!nL.empty()) {
						std::tuple<int, int, int> mapping = inputMapHandler(nL);
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
				throw std::runtime_error("Input map configuration failed, order of operations in 'mapping_config.txt' is incorrect. 'Input Map' should be the first mapping in the config file.");
			}
		}
	}
	else if (fullyConnected == 1) {
		while (mappingConfig_.isNextLine()) {
			std::string nextLine = mappingConfig_.readNextLine();
			if (!nextLine.empty()) {
				res = inputMapFCHandler(nextLine);
				break;
			}
		}
	}
	else {
		throw std::runtime_error("Input map configuration failed, FullyConnected was not set. Please make sure FullyConnected is set to Yes or No");
	}

	if (res.empty()) {
		throw std::runtime_error("Input map configuration failed, input map was empty.");
	}

	this->inputBeforeLayers = true;
	return res;
}

std::tuple<int, int, int> MappingConfigurator::inputMapHandler(std::vector<std::string> v)
{
	int inputNum = -1;
	int neuronInputNum = -1;
	int weight = 1; // Default weight if weight is not specified

	if (v[0] == "End") {
		return std::make_tuple(inputNum, neuronInputNum, weight);
	}
	
	if (v[0] != "Input:") {
		throw std::runtime_error("Input map configuration failed, invalid formatting of input mapping. Correct formatting is 'Input: #, Network: #' or 'Input: #, Network: #, Weight: #' ");
	}
	inputNum = std::stoi(v[1]);

	if (v[2] != "Network:") {
		throw std::runtime_error("Input map configuration failed, invalid formatting of input mapping. Correct formatting is 'Input: #, Network: #' or 'Input: #, Network: #, Weight: #'");
	}
	neuronInputNum = std::stoi(v[3]);

	if (v.size() > 5) { // 5 because without weight size is 4, with weight size is 6.
		if (v[4] == "Weight:") {
			weight = std::stoi(v[5]);
		}
		else {
			throw std::runtime_error("Input map configuration failed, invalid formatting of input mapping. Correct formatting is 'Input: #, Network: #' or 'Input: #, Network: #, Weight: #'");
		}
	}

	return std::make_tuple(inputNum, neuronInputNum, weight);
}

std::vector<std::tuple<int, int, int>> MappingConfigurator::inputMapFCHandler(std::string v)
{
	std::vector<std::tuple<int, int, int>> res;
	int numInputs = -1;
	int numNeuronsFirstLayer = -1;
	int weight = 1;

	if (v == "Input Map:") {
		while (mappingConfig_.isNextLine()) {
			std::vector<std::string> nextLine = mappingConfig_.readNextLineSplit(" ");
			if (!nextLine.empty()) {
				if (nextLine[0] == "End") {
					break;
				}
				else if (nextLine[0] == "Inputs:") {
					numInputs = std::stoi(nextLine[1]);
				}
				else if (nextLine[0] == "Layer") {
					if (nextLine[1] != "0") {
						throw std::runtime_error("Input map configuration failed, formatting error. Format should always be 'Layer 0 Neurons: #'");
					}
					if (nextLine[2] != "Neurons:") {
						throw std::runtime_error("Input map configuration failed, formatting error. Format should always be 'Layer 0 Neurons: #'");
					}
					numNeuronsFirstLayer = std::stoi(nextLine[3]);
				}
				else if (nextLine[0] == "DefaultWeight:") {
					if (nextLine.size() > 1) {
						weight = std::stoi(nextLine[1]);
					}
				}
			}
		}

		if (numInputs < 0 || numNeuronsFirstLayer < 0) {
			throw std::runtime_error("Input map configuration failed, invalid value specified for 'Inputs: #' or 'Layer 0 Neurons: #'");
		}

		int numInputsNetwork = numInputs * numNeuronsFirstLayer;
		int i = 0;

		for (int j = 0; j < numInputsNetwork; ++j) {
			res.push_back(std::make_tuple(i, j, weight));
			i += 1;
			i = i % numInputs;
		}
	}
	else {
		throw std::runtime_error("Input map configuration failed, order of operations in 'mapping_config.txt' is incorrect. 'Input Map' should be the first mapping in the config file.");
	}

	return res;
}

std::vector<std::tuple<int, int, int>> MappingConfigurator::createLayerMap()
{
	std::vector<std::tuple<int, int, int>> res;

	if (!this->inputBeforeLayers) {
		throw std::runtime_error("Layer map configuration failed, invalid order of operations. Input mapping first, then layer mapping");
	}

	while (mappingConfig_.isNextLine()) {
		std::vector<std::string> v = mappingConfig_.readNextLineSplit(" ");
		if (v.empty()) {
			continue;
		}
		if (v[0] == "Layer" && v[1] == "Map") {
			layerCounter += 1;
			if (layerCounter != std::stoi(v[2])) {
				throw std::runtime_error("Layer map configuration failed, numbering for each mapping between layers is out of order. Numbering should be 0, 1, 2, ...");
			}
			if (fullyConnected == 0) {
				while (mappingConfig_.isNextLine()) {
					std::vector<std::string> nL = mappingConfig_.readNextLineSplit(" ");
					if (!nL.empty()) {
						std::tuple<int, int, int> mapping = layerMapHandler(nL);
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
			else if (fullyConnected == 1) {
				while (mappingConfig_.isNextLine()) {
					res = layerMapFCHandler();
					break;
				}
				break;
			}
			else {
				throw std::runtime_error("Input map configuration failed, FullyConnected was not set. Please make sure FullyConnected is set to Yes or No");
			}
		}
		else {
			throw std::runtime_error("Layer map configuration failed, Layer map not found. Check if spelling and formatting is correct: 'Layer Map #:'");
		}
	}

	if (res.empty()) {
		throw std::runtime_error("Layer map configuration failed, layer map was empty.");
	}

	return res;
}

std::tuple<int, int, int> MappingConfigurator::layerMapHandler(std::vector<std::string> v)
{
	int firstLayerOutputNum = -1;
	int secondLayerInputNum = -1;
	int weight = 1; // Default weight if weight is not specified
	int c = layerCounter + 1; // Used for error checks

	if (v[0] == "End") {
		return std::make_tuple(firstLayerOutputNum, secondLayerInputNum, weight);
	}

	if (v[0] != "Layer") {
		throw std::runtime_error("Layer map configuration failed, invalid formatting of layer mapping. 'Layer:' Not found. Correct formatting is 'Layer n: #, Layer n+1: #' or 'Layer n: #, Layer n+1: #, Weight: #'");
	}

	int a = std::stoi(v[1]); //  Used for error checks
	if (a != layerCounter) {
		throw std::runtime_error("Layer map configuration failed, invalid formatting of layer mapping. Incorrect numbering of layers. Correct formatting is 'Layer n: #, Layer n+1: #' or 'Layer n: #, Layer n+1: #, Weight: #'");
	}
	firstLayerOutputNum = std::stoi(v[2]);

	if (v[3] != "Layer") {
		throw std::runtime_error("Layer map configuration failed, invalid formatting of layer mapping. 'Layer:' Not found. Correct formatting is 'Layer n: #, Layer n+1: #' or 'Layer n: #, Layer n+1: #, Weight: #'");
	}

	int b = std::stoi(v[4]); //  Used for error checks
	if (b != c) {
		throw std::runtime_error("Layer map configuration failed, invalid formatting of layer mapping. Incorrect numbering of layers. Correct formatting is 'Layer n: #, Layer n+1: #' or 'Layer n: #, Layer n+1: #, Weight: #'");
	}
	secondLayerInputNum = std::stoi(v[5]);

	if (v.size() > 7) { // 7 because without weight size is 6, with weight size is 8.
		if (v[6] == "Weight:") {
			weight = std::stoi(v[7]);
		}
		else {
			throw std::runtime_error("Layer map configuration failed, invalid formatting of layer mapping. Correct formatting is 'Layer n: #, Layer n+1: #' or 'Layer n: #, Layer n+1: #, Weight: #'");
		}
	}

	return std::make_tuple(firstLayerOutputNum, secondLayerInputNum, weight);
}

std::vector<std::tuple<int, int, int>> MappingConfigurator::layerMapFCHandler()
{
	std::vector<std::tuple<int, int, int>> res;
	int numNeuronsFirstLayer = -1;
	int numNeuronsSecondLayer = -1;
	int weight = 1;

	while (mappingConfig_.isNextLine()) {
		std::vector<std::string> nextLine = mappingConfig_.readNextLineSplit(" ");
		if (!nextLine.empty()) {
			if (nextLine[0] == "End") {
				break;
			}
			else if (nextLine[0] == "Layer") {
				if (std::stoi(nextLine[1]) == layerCounter) {
					if (nextLine[2] != "Neurons:") {
						throw std::runtime_error("Layer map configuration failed, formatting error. Format should always be 'Layer n Neurons: #' \n");
					}
					numNeuronsFirstLayer = std::stoi(nextLine[3]);
				}
				else if (std::stoi(nextLine[1]) == (layerCounter + 1)) {
					if (nextLine[2] != "Neurons:") {
						throw std::runtime_error("Layer map configuration failed, formatting error. Format should always be 'Layer n+1 Neurons: #' \n");
					}
					numNeuronsSecondLayer = std::stoi(nextLine[3]);
				}
				else {
					throw std::runtime_error("Layer map configuration failed, formatting error. Numbers after 'Layer' not in order.");
				}
			}
			else if (nextLine[0] == "DefaultWeight:") {
				if (nextLine.size() > 1) {
					weight = std::stoi(nextLine[1]);
				}
			}
		}
	}

	if (numNeuronsFirstLayer < 0 || numNeuronsSecondLayer < 0) {
		throw std::runtime_error("Layer map configuration failed, invalid value specified for 'Layer n Neurons: #' or 'Layer n+1 Neurons: #' \n");
	}

	int numInputsSecondLayer = numNeuronsFirstLayer * numNeuronsSecondLayer;
	int i = 0;

	for (int j = 0; j < numInputsSecondLayer; ++j) {
		res.push_back(std::make_tuple(i, j, weight));
		i += 1;
		i = i % numNeuronsFirstLayer;
	}

	return res;
}
