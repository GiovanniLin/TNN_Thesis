#pragma once
#pragma once
#include "FileReader.h"
#include <iostream>
#include <vector>
#include "Layer.h"

class WeightConfigurator
{
public:
	WeightConfigurator(std::string weightConfig);
	std::vector<std::vector<std::vector<double>>> getWeights();
	void readWeights(std::vector<Layer>& layers);
private:
	FileReader weightConfig_;
	std::vector<std::vector<std::vector<double>>> weights;
};

