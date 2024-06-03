#pragma once
#include "FileReader.h"
#include <iostream>
#include <vector>
#include <tuple>

class MappingConfigurator
{
public:
	MappingConfigurator(std::string mappingConfig);
	std::vector<std::tuple<int, int, int>> createInputMap();
	std::tuple<int, int, int> inputMapHandler(std::vector<std::string> v);
	std::vector<std::tuple<int, int, int>> createLayerMap();
	std::tuple<int, int, int> layerMapHandler(std::vector<std::string> v);
private:
	FileReader mappingConfig_;
	bool inputBeforeLayers = false;
	int layerCounter = -1;
};

