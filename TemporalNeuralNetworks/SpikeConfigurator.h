#pragma once
#include "FileReader.h"
#include <iostream>
#include <vector>
#include <tuple>

class SpikeConfigurator
{
public:
	SpikeConfigurator(std::string spikeConfig);
	std::vector<std::tuple<int, int>> createSpikes();
	std::tuple<int, int> spikeHandler(std::vector<std::string> v);
private:
	FileReader spikeConfig_;
};

