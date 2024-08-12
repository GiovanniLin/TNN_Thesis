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
#include "STDPConfigurator.h"
#include "../Environment/Environment.h"
#include "WeightConfigurator.h"

void runEpisodes(Environment& env, std::vector<Layer>& layers, NetworkConfigurator& networkConfig, STDPConfigurator& stdpConfig,
	std::ofstream& myfile, bool inputs[], std::vector<std::tuple<int, int, int>> inputMap, std::vector<std::tuple<int, int, int>> layerMap);

int run(
	int cycles, 
	std::vector<Layer>& layers,
	bool inputs[],
	std::vector<std::tuple<int, int, int>> inputMap,
	std::vector<std::tuple<int, int, int>> layerMap,
	int numInputs,
	std::vector<std::tuple<int, int>> spikes);

bool sortbysec(const std::tuple<int, int>& a, const std::tuple<int, int>& b);

void connectInputs(bool inputs[], int numInputs, Layer& inputLayer, std::vector<std::tuple<int, int, int>> inputMap);

void connectLayers(Layer& firstLayer, Layer& secondLayer, std::vector<std::tuple<int, int, int>> layerMap);

std::tuple<int, int> getNeuronInputIndex(Layer& inputLayer, std::tuple<int, int, int> mapping);

std::vector<std::vector<int>> generateSpikes(std::vector<std::tuple<int, int>> spikes, int simLength);

void fireSpikes(std::vector<std::vector<int>> spikes, bool inputs[], int time);

void resetSpikes(bool inputs[], int numInputs);

std::vector<std::tuple<int, int>> createSpikesFromEncoding(std::vector<int> encoding);