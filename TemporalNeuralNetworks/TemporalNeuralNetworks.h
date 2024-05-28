#pragma once
#include <iostream>
#include <string>
#include <algorithm>
#include <tuple>
#include <vector>
#include <variant>
#include "Neuron.h"
#include "FileReader.h"
#include "NetworkConfigurator.h"

int run(
	int cycles, 
	std::vector<std::vector<Neuron>>& layers,
	bool inputs[], 
	std::vector<std::tuple<int, int, int>> inputMap, 
	std::vector<std::tuple<int, int, int>> layerMap, 
	std::vector<std::tuple<int, int>> spikes);

bool sortbysec(const std::tuple<int, int>& a, const std::tuple<int, int>& b);

void connectInputs(std::vector<Neuron>& inputLayer, bool inputs[], std::vector<std::tuple<int, int, int>> inputMap);

void connectLayers(std::vector<Neuron>& firstLayer, std::vector<Neuron>& secondLayer, std::vector<std::tuple<int, int, int>> layerMap);

std::tuple<int, int> getNeuronInputIndex(std::vector<Neuron>& inputLayer, std::tuple<int, int, int> mapping);

std::vector<std::vector<int>> generateSpikes(std::vector<std::tuple<int, int>> spikes, int simLength);

void fireSpikes(std::vector<std::vector<int>> spikes, bool inputs[], int time);

void resetSpikes(bool inputs[]);