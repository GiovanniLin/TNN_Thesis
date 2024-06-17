#pragma once
#include "FileReader.h"
#include <iostream>
#include <vector>
#include <tuple>

class MappingConfigurator
{
public:
	MappingConfigurator(std::string mappingConfig);
	void setFullyConnected(int fullyConnected);
	void configHandler(std::vector<std::string> v);
	std::vector<std::tuple<int, int, int>> createInputMap();
	std::tuple<int, int, int> inputMapHandler(std::vector<std::string> v);
	std::vector<std::tuple<int, int, int>> inputMapFCHandler(std::string v);
	std::vector<std::tuple<int, int, int>> createLayerMap();
	std::tuple<int, int, int> layerMapHandler(std::vector<std::string> v);
	std::vector<std::tuple<int, int, int>> layerMapFCHandler();
private:
	FileReader mappingConfig_;
	bool inputBeforeLayers = false;
	int layerCounter = -1;
	int fullyConnected = -1; // 0 is No, 1 is Yes, -1 is value not set
};

