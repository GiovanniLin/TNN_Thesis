#include "MappingConfigurator.h"

MappingConfigurator::MappingConfigurator(std::string mappingConfig) : mappingConfig_(FileReader::FileReader(mappingConfig))
{
	//First line of the mapping_config.txt must be "Mapping Config:"
	std::string firstLine = mappingConfig_.readNextLine();
	if (firstLine.empty() || firstLine != "Mapping Config:") {
		throw std::runtime_error("Mapping configuration failed, file passed to MappingConfigurator is not a mapping configuration. Make sure the first line of the text file says 'Mapping Config:'");
	}
}

std::vector<std::tuple<int, int, int>> MappingConfigurator::createInputMap()
{
	std::vector<std::tuple<int, int, int>> res;

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
		else if (v[0] == "Layer" && v[1] == "Map") {
			layerCounter += 1;
			if (layerCounter != std::stoi(v[2])) {
				throw std::runtime_error("Layer map configuration failed, numbering for each mapping between layers is out of order. Numbering should be 0, 1, 2, ...");
			}
			//std::cout << "layerCounter: " << layerCounter << " \n";
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
		}
		else {
			throw std::runtime_error("Layer map configuration failed, Layer map not found. Check if spelling and formatting is correct: 'Layer Map #:'");
		}
	}

	if (res.empty()) {
		throw std::runtime_error("Layer map configuration failed, input map was empty.");
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