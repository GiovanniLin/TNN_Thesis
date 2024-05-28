#pragma once
#include "FileReader.h"
#include <iostream>
#include <vector>
#include "Neuron.h"

class NetworkConfigurator
{
public:
	NetworkConfigurator(FileReader& networkConfig);
	int getNumInputs();
	void setNumInputs(int numInputs);
	int getNumLayers();
	void setNumLayers(int numLayers);
	int getIFType();
	void setIFType(int ifType);
	std::vector<std::vector<Neuron>> createLayers(FileReader& networkConfig);
	void configHandler(std::vector<std::string> v);
	std::vector<Neuron> layerHandler(std::vector<std::string> v, FileReader& networkConfig);
private:
	int numInputs = -1;
	int numLayers = -1;
	int ifType = -1;
};

