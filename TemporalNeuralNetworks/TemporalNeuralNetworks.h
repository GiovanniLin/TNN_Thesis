#pragma once
#include <iostream>
#include <string>
#include <algorithm>
#include <functional>
#include <tuple>
#include <vector>
#include <variant>
#include "Neuron.h"
#include "FileReader.h"
#include "NetworkConfigurator.h"
#include "MappingConfigurator.h"
#include "SpikeConfigurator.h"
#include "Layer.h"

int run(
	int cycles, 
	std::vector<Layer>& layers,
	bool inputs[], 
	std::vector<std::tuple<int, int, int>> inputMap, 
	std::vector<std::tuple<int, int, int>> layerMap, 
	std::vector<std::tuple<int, int>> spikes);

bool sortbysec(const std::tuple<int, int>& a, const std::tuple<int, int>& b);

void connectInputs(bool inputs[], Layer& inputLayer, std::vector<std::tuple<int, int, int>> inputMap);

void connectLayers(Layer& firstLayer, Layer& secondLayer, std::vector<std::tuple<int, int, int>> layerMap);

std::tuple<int, int> getNeuronInputIndex(Layer& inputLayer, std::tuple<int, int, int> mapping);

std::vector<std::vector<int>> generateSpikes(std::vector<std::tuple<int, int>> spikes, int simLength);

void fireSpikes(std::vector<std::vector<int>> spikes, bool inputs[], int time);

void resetSpikes(bool inputs[]);