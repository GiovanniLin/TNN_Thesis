// TemporalNeuralNetworks.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "TemporalNeuralNetworks.h"

const std::string INDENT = "    ";

int main()
{
    std::cout << "Reading Network Configuration \n";
    NetworkConfigurator networkConfig("network_config.txt");

    std::cout << "Creating network layers \n\n";
    std::vector<Layer> layers;

    try {
        layers = networkConfig.createLayers();
    }
    catch (std::runtime_error& e) {
        std::cout << e.what();
        return 1;
    }

    std::cout << "Network Configuration: \n";
    std::cout << INDENT << "Number of Inputs: " << networkConfig.getNumInputs() << "\n";
    std::cout << INDENT << "Number of Layers: " << networkConfig.getNumLayers() << " \n";
    std::cout << INDENT << "IF Type: " << networkConfig.getIFType() << "\n\n";

    std::cout << "Reading STDP Configuration \n\n";
    STDPConfigurator stdpConfig("stdp_config.txt");

    std::cout << "STDP Configuration: \n";
    std::cout << INDENT << "C-TNN Parameters: \n";
    std::cout << INDENT << INDENT << "Capture: " << stdpConfig.getCapture() << "\n";
    std::cout << INDENT << INDENT << "Backoff: " << stdpConfig.getBackoff() << "\n";
    std::cout << INDENT << INDENT << "Search: " << stdpConfig.getSearch() << "\n";
    std::cout << INDENT << "R-TNN Parameters: \n";
    std::cout << INDENT << INDENT << "Reward Potentiation and Depression: " << stdpConfig.getRewardPD() << "\n";
    std::cout << INDENT << INDENT << "Reward Window: " << stdpConfig.getRewardW() << "\n";
    std::cout << INDENT << INDENT << "Punishment Potentiation and Depression: " << stdpConfig.getPunishmentPD() << "\n";
    std::cout << INDENT << INDENT << "Punishment Window: " << stdpConfig.getPunishmentW() << "\n\n";

    std::cout << "Reading Mapping Configuration \n\n";
    MappingConfigurator mappingConfig("mapping_config.txt");

    std::vector<std::tuple<int, int, int>> inputMap;

    std::cout << "Creating input map \n\n";
    try {
        inputMap = mappingConfig.createInputMap();
    }
    catch (std::runtime_error& e) {
        std::cout << e.what();
        return 1;
    }

    std::cout << "Input Mapping: \n";
    for (size_t i = 0; i < inputMap.size(); ++i) {
        std::cout << INDENT << "Input: " << std::get<0>(inputMap[i]) << ", Network: " << std::get<1>(inputMap[i]) << ", Weight: " << std::get<2>(inputMap[i]) << "\n";
    }
    std::cout << "\n";

    std::vector<std::tuple<int, int, int>> layerMap;

    std::cout << "Creating layer map \n\n";
    try {
        layerMap = mappingConfig.createLayerMap();
    }
    catch (std::runtime_error& e) {
        std::cout << e.what();
        return 1;
    }

    std::cout << "Layer Mapping: \n";
    for (size_t i = 0; i < layerMap.size(); ++i) {
        std::cout << INDENT << "Layer 0: " << std::get<0>(layerMap[i]) << ", Layer 1: " << std::get<1>(layerMap[i]) << ", Weight: " << std::get<2>(layerMap[i]) << "\n";
    }
    std::cout << "\n";

    std::cout << "Reading Spike Configuration \n\n";
    SpikeConfigurator spikeConfig("spike_config.txt");

    std::vector<std::tuple<int, int>> spikes;

    std::cout << "Creating spikes \n\n";
    try {
        spikes = spikeConfig.createSpikes();
    }
    catch (std::runtime_error& e) {
        std::cout << e.what();
        return 1;
    }

    std::cout << "Spikes: \n";
    for (size_t i = 0; i < spikes.size(); ++i) {
        std::cout << INDENT << "Input: " << std::get<0>(spikes[i]) << ", Time: " << std::get<1>(spikes[i]) << "\n";
    }
    std::cout << "\n";

    // System (not the neuron) inputs
    int numInputs = networkConfig.getNumInputs();
    bool* inputs = new bool[numInputs];

    for (int i = 0; i < numInputs; ++i) {
        inputs[i] = false;
    }

    std::cout << "Created Inputs \n";

    int output = run(15, layers, inputs, inputMap, layerMap, spikes);
    if (output > 0) {
        std::cout << "Output spike generated at time " << output << "\n\n";
    }
    else {
        std::cout << "No output spike \n\n";
    }

    for (int i = 0; i < layers.size(); ++i) {
        for (int j = 0; j < layers[i].neurons.size(); ++j) {
            std::cout << "Final body potential of Layer " << i << " Neuron " << j << ": " << layers[i].neurons[j].currentBodyPotential() << " \n";
        }
    }

    // Delete dynamically allocated array for inputs
    delete[] inputs;

    return 0;
}

int run(
    int cycles, 
    std::vector<Layer>& layers, 
    bool inputs[], 
    std::vector<std::tuple<int, int, int>> inputMap,
    std::vector<std::tuple<int, int, int>> layerMap,
    std::vector<std::tuple<int, int>> spikes)
{
    int outputTime = 0;
    bool stopRunning = false;

    std::cout << "Connecting inputs \n";
    connectInputs(inputs, layers[0], inputMap);

    std::cout << "Connecting layers \n";
    connectLayers(layers[0], layers[1], layerMap);


    std::vector<std::reference_wrapper<bool>> finalOutput;

    for (int i = 0; i < layers[layers.size() - 1].neurons.size(); ++i) {
        finalOutput.push_back(layers[layers.size() - 1].neurons[i].output);
    }

    std::cout << "Sorting Spikes \n";
    std::sort(spikes.begin(), spikes.end(), sortbysec);

    std::cout << "Generating spike times for each inputs \n";
    std::vector<std::vector<int>> simSpikes = generateSpikes(spikes, cycles);

    // Add white space before starting for loop
    std::cout << "\n";
    
    resetSpikes(inputs); // Make sure everything is false

    std::cout << "Now running \n\n";
    for (int i = 0; i < cycles; i++) {
        fireSpikes(simSpikes, inputs, i);

        for (int j = 0; j < layers.size(); j++) {
            layers[j].checkNeuronIFs();
            layers[j].checkNeuronSpikes();
            layers[j].checkNeuronThresholds();
            
            std::vector<int> outputs = layers[j].checkOutputs();

            if (!outputs.empty()) {
                for (auto k : outputs) {
                    std::cout << "Layer " << j << ", Neuron " << k << " generated output spike at time " << i << "\n";
                }
            }
        }

        for (int j = 0; j < layers.size(); ++j) {
            for (int k = 0; k < layers[j].neurons.size(); ++k) {
                std::cout << "Body potential at time " << i << " of Layer " << j << " Neuron " << k << ": " << layers[j].neurons[k].currentBodyPotential() << " \n";
            }
        }

        for (int j = 0; j < finalOutput.size(); ++j) {
            if (finalOutput[j]) {
                outputTime = i;
                std::cout << "Output from Layer " << (layers.size() - 1) << " Neuron " << j << " \n";
                std::cout << "\n";
                stopRunning = true;
                break;
            }
        }

        if (stopRunning) {
            break;
        }

        resetSpikes(inputs); // Falling edge of each spike, for spike validity

        for (int j = 0; j < layers.size(); j++) {
            layers[j].removeOutputSpikes(); // Falling edge of each spike, for spike validity
        }

        // Add white space after 1 cycle
        std::cout << "\n";
    }
    return outputTime;
}

bool sortbysec(const std::tuple<int, int>& a, const std::tuple<int, int>& b)
{
    return (std::get<1>(a) < std::get<1>(b));
}

void connectInputs(bool inputs[], Layer& inputLayer, std::vector<std::tuple<int, int, int>> inputMap)
{
    for (auto mapping : inputMap) {

        int inputIndex = std::get<0>(mapping);

        std::tuple<int, int> neuronIndices = getNeuronInputIndex(inputLayer, mapping);

        int neuronIndex = std::get<0>(neuronIndices);

        int neuronInputIndex = std::get<1>(neuronIndices);

        int weight = std::get<2>(mapping);
        bool* ptr = &(inputs[inputIndex]);

        inputLayer.neurons[neuronIndex].overwriteInput(neuronInputIndex, weight, ptr);
    }
}

// Adjust later for more than two layers.
void connectLayers(Layer& firstLayer, Layer& secondLayer, std::vector<std::tuple<int, int, int>> layerMap) 
{
    for (auto mapping : layerMap) {
        int firstLayerNeuronIndex = std::get<0>(mapping);

        std::tuple<int, int> secondLayerNeuronIndices = getNeuronInputIndex(secondLayer, mapping);

        int secondLayerNeuronIndex = std::get<0>(secondLayerNeuronIndices);

        int secondLayerNeuronInputIndex = std::get<1>(secondLayerNeuronIndices);

        int weight = std::get<2>(mapping);
        bool* ptr = &(firstLayer.neurons[firstLayerNeuronIndex].output);

        secondLayer.neurons[secondLayerNeuronIndex].overwriteInput(secondLayerNeuronInputIndex, weight, ptr);
    }
}

std::tuple<int, int> getNeuronInputIndex(Layer& inputLayer, std::tuple<int, int, int> mapping)
{
    int neuronIndex = 0;
    int neuronInputIndex = std::get<1>(mapping);

    for (auto neuron : inputLayer.neurons) {
        if (neuron.inputs.size() <= neuronInputIndex) {
            neuronInputIndex -= neuron.inputs.size();
            neuronIndex += 1;
        }
        else {
            break;
        }
    }
    return std::tuple<int, int>(neuronIndex, neuronInputIndex);
}

std::vector<std::vector<int>> generateSpikes(std::vector<std::tuple<int, int>> spikes, int simLength)
{
    std::vector<std::vector<int>> res(simLength);
    for (auto spike : spikes) {
        int inputIndex = std::get<0>(spike);
        int timeIndex = std::get<1>(spike);

        res[timeIndex].push_back(inputIndex);
    }
    return res;
}

void fireSpikes(std::vector<std::vector<int>> spikes, bool inputs[], int time)
{
    for (auto spike : spikes[time]) {

        std::cout << "Firing spike at time:  " << time << " on input " << spike << " \n";
        inputs[spike] = true;
    }
}

void resetSpikes(bool inputs[])
{
    for (size_t i = 0; i < (sizeof(inputs)/sizeof(bool)); i++) {
        inputs[i] = false;
    }
}

