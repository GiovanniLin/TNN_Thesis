#pragma once
#include "FileReader.h"
#include <iostream>
#include <vector>
#include "Neuron.h"
#include "Layer.h"

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
	int getIFThreshold();
	void setIFThreshold(int ifThreshold);
	void setFullConfigure(int fullConfigure);
	std::vector<Layer> createLayers();
	void configHandler(std::vector<std::string> v);
	Layer layerHandler(std::vector<std::string> v);
	~NetworkConfigurator();
private:
	int numInputs = -1;
	int numLayers = -1;
	int ifType = -1;
	int ifThreshold = -1;
	int fullConfigure = -1; // 0 is No, 1 is Yes, -1 is value not set
	int neuronCounter = 0;
	int numNeuronsPrevLayer = -1;
	FileReader networkConfig_;
};

