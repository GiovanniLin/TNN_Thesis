#pragma once
#include "FileReader.h"
#include <iostream>
#include <vector>
#include "Neuron.h"
#include "layer.h"

class NetworkConfigurator
{
public:
	NetworkConfigurator(std::string networkConfig);
	int getNumInputs();
	void setNumInputs(int numInputs);
	int getNumLayers();
	void setNumLayers(int numLayers);
	int getIFType();
	void setIFType(int ifType);
	std::vector<Layer> createLayers();
	void configHandler(std::vector<std::string> v);
	Layer layerHandler(std::vector<std::string> v);
	~NetworkConfigurator();
private:
	int numInputs = -1;
	int numLayers = -1;
	int ifType = -1;
	int neuronCounter = 0;
	FileReader networkConfig_;
};

