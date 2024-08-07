// TemporalNeuralNetworks.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "TemporalNeuralNetworks.h"

const std::string INDENT = "    ";
int reward = 0;

int main()
{
    Environment env;
    env.testPrint();

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
    for (int i = 0; i < layers.size(); ++i) {
        if (layers[i].getTypeTNN() == 0) {
            std::cout << INDENT << "Layer " << i << " Type: C-TNN \n";
        }
        else if (layers[i].getTypeTNN() == 1) {
            std::cout << INDENT << "Layer " << i << " Type: R-TNN \n";
        }
        else {
            std::cout << INDENT << "Layer " << i << " Type: Unknown, Please check validity of parameter 'Type: #' \n";
        }
        if (layers[i].getReadWeights()) {
            std::cout << INDENT << "Layer " << i << " weights read from config \n";
        }
        else {
            std::cout << INDENT << "Layer " << i << " weights are default \n";
        }
    }
    std::cout << INDENT << "IF Type: " << networkConfig.getIFType() << "\n";
    std::cout << INDENT << "Environment Variables: " << networkConfig.getIntervals().size() << "\n";
    std::cout << INDENT << "m-Hot Code: " << networkConfig.getMHotCode() << "\n";
    std::cout << INDENT << "Encoding Intervals: \n";
    for (int i = 0; i < networkConfig.getIntervals().size(); ++i) {
        std::cout << INDENT << INDENT << "Environment Variable " << i << ": [";
        for (int j = 0; j < networkConfig.getIntervals()[i].size(); ++j) {
            if (j + 1 == networkConfig.getIntervals()[i].size()) {
                std::cout << networkConfig.getIntervals()[i][j];
            }
            else {
                std::cout << networkConfig.getIntervals()[i][j] << ", ";
            }
        }
        std::cout << "]\n";
    }
    std::cout << INDENT << "\n";

    bool readWeights = false;

    for (int i = 0; i < layers.size(); ++i) {
        if (layers[i].getReadWeights()) {
            readWeights = true;
            break;
        }
    }

    std::cout << "Reading STDP Configuration \n\n";
    STDPConfigurator stdpConfig("stdp_config.txt");

    std::cout << "STDP Configuration: \n";
    std::cout << INDENT << "C-TNN Parameters: \n";
    std::cout << INDENT << INDENT << "Capture: " << stdpConfig.getCapture() << "\n";
    std::cout << INDENT << INDENT << "Backoff: " << stdpConfig.getBackoff() << "\n";
    std::cout << INDENT << INDENT << "Search: " << stdpConfig.getSearch() << "\n";
    std::cout << INDENT << "R-TNN Parameters: \n";
    std::cout << INDENT << INDENT << "Reward Potentiation: " << stdpConfig.getRewardP() << "\n";
    std::cout << INDENT << INDENT << "Reward Depression: " << stdpConfig.getRewardD() << "\n";
    std::cout << INDENT << INDENT << "Reward Window: " << stdpConfig.getRewardW() << "\n";
    std::cout << INDENT << INDENT << "Punishment Potentiation: " << stdpConfig.getPunishmentP() << "\n";
    std::cout << INDENT << INDENT << "Punishment Depression: " << stdpConfig.getPunishmentD() << "\n";
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

    //std::cout << "Input Mapping: \n";
    //for (size_t i = 0; i < inputMap.size(); ++i) {
    //    std::cout << INDENT << "Input: " << std::get<0>(inputMap[i]) << ", Network: " << std::get<1>(inputMap[i]) << ", Weight: " << std::get<2>(inputMap[i]) << "\n";
    //}
    //std::cout << "\n";

    std::vector<std::tuple<int, int, int>> layerMap;

    std::cout << "Creating layer map \n\n";
    try {
        layerMap = mappingConfig.createLayerMap();
    }
    catch (std::runtime_error& e) {
        std::cout << e.what();
        return 1;
    }

    //std::cout << "Layer Mapping: \n";
    //for (size_t i = 0; i < layerMap.size(); ++i) {
    //    std::cout << INDENT << "Layer 0: " << std::get<0>(layerMap[i]) << ", Layer 1: " << std::get<1>(layerMap[i]) << ", Weight: " << std::get<2>(layerMap[i]) << "\n";
    //}
    //std::cout << "\n";

    //std::cout << "Reading Spike Configuration \n\n";
    //SpikeConfigurator spikeConfig("spike_config.txt");

    //std::vector<std::tuple<int, int>> spikes;

    //std::cout << "Creating spikes \n\n";
    //try {
    //    spikes = spikeConfig.createSpikes();
    //}
    //catch (std::runtime_error& e) {
    //    std::cout << e.what();
    //    return 1;
    //}

    //std::cout << "Spikes: \n";
    //for (size_t i = 0; i < spikes.size(); ++i) {
    //    std::cout << INDENT << "Input: " << std::get<0>(spikes[i]) << ", Time: " << std::get<1>(spikes[i]) << "\n";
    //}
    //std::cout << "\n";

    // System (not the neuron) inputs
    int numInputs = networkConfig.getNumInputs();
    bool* inputs{ new bool[numInputs] {} };

    for (int i = 0; i < numInputs; ++i) {
        inputs[i] = false;
    }

    std::cout << "Created Inputs \n\n";

    //double angle = 10.5;
    //std::vector<int> encoding = networkConfig.getEncoding(0, angle);
    //std::cout << "Encoding test: \n";
    //std::cout << INDENT << "Angle: " << angle << " \n";
    //std::cout << INDENT << "Encodings: " << " \n";
    //for (int i = 0; i < encoding.size(); ++i) {
    //    std::cout << INDENT << INDENT << "Spike on Input " << encoding[i] << " \n";
    //}
    //std::cout << "\n";

    //env.testMath();

    int cycleCounter = 0;

    int episodeCounter = 0;

    int episodeLimit = 0;

    bool trainingMode = env.trainingMode;

    if (trainingMode) {
        episodeLimit = env.trainingEpisodes;
    }
    else
    {
        episodeLimit = env.testEpisodes;
    }

    std::cout << "Connecting inputs \n";
    connectInputs(inputs, numInputs, layers[0], inputMap);

    std::cout << "Connecting layers \n\n";
    connectLayers(layers[0], layers[1], layerMap);

    if (readWeights) {
        WeightConfigurator weightConfig("weight_config.txt");
        weightConfig.readWeights(layers);
        std::vector<std::vector<std::vector<double>>> weights = weightConfig.getWeights();
        for (int i = 0; i < layers.size(); ++i) {
            if (layers[i].getReadWeights()) {
                //std::cout << "Setting Weights for Layer " << i << "\n\n";
                layers[i].setWeights(weights[i]);
            }
        }
        //for (int i = 0; i < weights.size(); ++i) {
        //    if (weights[i].empty()) {
        //        std::cout << "No weights read for Layer " << i << " \n";
        //    }
        //    for (int j = 0; j < weights[i].size(); ++j) {
        //        for (int k = 0; k < weights[i][j].size(); ++k) {
        //            std::cout << "Layer " << i << ", Neuron " << j << ", Synapse: " << k << ", Weight: " << weights[i][j][k] << "\n";
        //        }
        //    }
        //}
        //std::cout << "\n";
    }

    while (episodeCounter < episodeLimit) {

        for (int i = 0; i < layers.size(); ++i) {
            if (i == 0) {
                layers[i].initializeVectors(numInputs, layers[i].neurons.size());
            }
            else {
                layers[i].initializeVectors(layers[i - 1].neurons.size(), layers[i].neurons.size());
            }
        }

        double inputAngle = env.getState().getAngle();
        std::vector<int> encodingAngle = networkConfig.getEncoding(0, inputAngle);

        //env.printState();

        //std::cout << "Spikes: " << " \n";
        //for (int i = 0; i < encodingAngle.size(); ++i) {
        //    std::cout << INDENT << "Spike on Input " << encodingAngle[i] << " \n";
        //}
        std::vector<std::tuple<int, int>> spikes = createSpikesFromEncoding(encodingAngle);
        //std::cout << "\n";

        int output = run(30, layers, inputs, inputMap, layerMap, numInputs, spikes);

        //if (output >= 0) {
        //    std::cout << "Spike from output: " << output << "\n\n";
        //}
        //else {
        //    std::cout << "No output spike, randomly generating a value. \n";
        //    output = std::rand() % layers[layers.size() - 1].neurons.size();
        //    std::cout << "Randomly generated output: " << output << "\n\n";
        //}

        bool action = env.decode(output);
        bool outOfBounds = env.stepState(action);

        reward = env.determineReward(cycleCounter, episodeCounter);

        //if (reward == -1) {
        //    std::cout << "Negative reward \n\n";
        //}
        //else if (reward == 1) {
        //    std::cout << "Positive reward \n\n";
        //}
        //else {
        //    std::cout << "No reward \n\n";
        //}
        
        if (trainingMode) {
            for (int i = 0; i < layers.size(); ++i) {
                //for (int j = 0; j < layers[i].neurons.size(); ++j) {
                //    std::cout << "Final body potential of Layer " << i << " Neuron " << j << ": " << layers[i].neurons[j].currentBodyPotential() << " \n";
                //}
                layers[i].updateWeights(stdpConfig, reward);
            }
        }

        cycleCounter += 1;

        if (cycleCounter >= env.cycleLimit) {
            outOfBounds = true;
        }

        if (outOfBounds) {
            if (cycleCounter >= env.cycleLimit) {
                std::cout << "Sim has reached cycleLimit \n";
            }
            else {
                std::cout << "Sim out of bounds \n";
            }
            
            std::cout << "Final State of the Environment: \n";
            env.printState();

            std::cout << "Number of cycles simulated: " << (cycleCounter - 1) << " \n\n";
            if (!trainingMode) {
                env.avgCycles.push_back((double)(cycleCounter - 1));
            }

            std::cout << "End of episode " << episodeCounter << " \n\n";
            episodeCounter += 1;
            if (episodeCounter >= episodeLimit) {
                if (trainingMode) {
                    trainingMode = false;
                    episodeCounter = 0;
                    episodeLimit = env.testEpisodes;
                    std::cout << "Training ended, now going to test mode. \n\n";
                }
            }
            env.resetState(env.randomAngle);
            cycleCounter = 0;
            
        }


        resetSpikes(inputs, numInputs);
        for (int i = 0; i < layers.size(); ++i) {
            layers[i].resetNeurons();
        }
    }

    double avgCycles = 0;
    for (int i = 0; i < env.avgCycles.size(); ++i) {
        avgCycles += env.avgCycles[i];
    }
    std::cout << "All episodes simulated. \n";
    std::cout << "Average Number of succesfull cycles for seed " << env.getState().seed << ": " << (avgCycles / env.avgCycles.size()) << "\n";
    std::cout << "Total Number of succesfull cycles for seed " << env.getState().seed << ": " << (avgCycles) << "\n\n";


    for (int i = 0; i < layers.size(); ++i) {
        //for (int j = 0; j < layers[i].getInputTime().size(); j++) {
        //    std::cout << "Input spike on Layer " << i << " on input " << j << " at time " << layers[i].getInputTime()[j] << " \n";
        //}
        //for (int j = 0; j < layers[i].getOutputTime().size(); j++) {
        //    std::cout << "Output spike on Layer " << i << " on output " << j << " at time " << layers[i].getOutputTime()[j] << " \n";
        //}
        //for (int j = 0; j < layers[i].getDecayCounters().size(); j++) {
        //    for (int k = 0; k < layers[i].getDecayCounters()[j].size(); ++k) {
        //        std::cout << "Decay counter on Layer " << i << " Input " << j << " Neuron " << k << ": " << layers[i].getDecayCounters()[j][k] << " \n";
        //    }
        //}
        for (int j = 0; j < layers[i].neurons.size(); ++j) {
            for (int k = 0; k < layers[i].neurons[j].inputs.size(); ++k) {
                std::cout << "Final weight of Layer " << i << " Neuron " << j << " Input " << k << " : " << layers[i].neurons[j].inputs[k]->getWeight() << " \n";
            }
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
    int numInputs,
    std::vector<std::tuple<int, int>> spikes)
{
    int res = -1;
    bool stopRunning = false;

    std::vector<std::reference_wrapper<bool>> finalOutput;

    for (int i = 0; i < layers[layers.size() - 1].neurons.size(); ++i) {
        finalOutput.push_back(layers[layers.size() - 1].neurons[i].output);
    }

    //std::cout << "Sorting Spikes \n";
    std::sort(spikes.begin(), spikes.end(), sortbysec);

    //std::cout << "Generating spike times for each inputs \n";
    std::vector<std::vector<int>> simSpikes = generateSpikes(spikes, cycles);

    // Add white space before starting for loop
    // std::cout << "\n";
    
    resetSpikes(inputs, numInputs); // Make sure everything is false

    //std::cout << "Now running \n\n";
    for (int i = 0; i < cycles; i++) {
        fireSpikes(simSpikes, inputs, i);
        for (int j = 0; j < numInputs; ++j) {
            if (inputs[j]) {
                layers[0].setInputTime(j, i);
                layers[0].resetCounters(j);
            }
        }

        for (int j = 0; j < layers.size(); j++) {
            layers[j].checkNeuronIFs();
            layers[j].checkNeuronSpikes(i);
            layers[j].checkNeuronThresholds(i);
            
            std::vector<int> outputs = layers[j].checkOutputs();

            if (!outputs.empty()) {
                for (auto k : outputs) {
                    //std::cout << "Layer " << j << ", Neuron " << k << " generated output spike at time " << i << "\n\n";
                    if (j + 1 < layers.size()) {
                        layers[j + 1].setInputTime(k, i);
                        layers[j + 1].resetCounters(k);
                    }
                }
            }
        }

        //for (int j = 0; j < layers.size(); ++j) {
        //    for (int k = 0; k < layers[j].neurons.size(); ++k) {
        //        std::cout << "Body potential at time " << i << " of Layer " << j << " Neuron " << k << ": " << layers[j].neurons[k].currentBodyPotential() << " \n";
        //    }
        //}

        for (int j = 0; j < finalOutput.size(); ++j) {
            if (finalOutput[j]) {
                res = j;
                //std::cout << "Output from Layer " << (layers.size() - 1) << " Neuron " << j << " \n";
                //std::cout << "\n";
                stopRunning = true;
                break;
            }
        }

        if (stopRunning) {
            break;
        }

        resetSpikes(inputs, numInputs); // Falling edge of each spike, for spike validity

        for (int j = 0; j < layers.size(); j++) {
            layers[j].incrementCounters();
            layers[j].removeOutputSpikes(); // Falling edge of each spike, for spike validity
        }
    }
    return res;
}

bool sortbysec(const std::tuple<int, int>& a, const std::tuple<int, int>& b)
{
    return (std::get<1>(a) < std::get<1>(b));
}

void connectInputs(bool inputs[], int numInputs, Layer& inputLayer, std::vector<std::tuple<int, int, int>> inputMap)
{
    for (auto mapping : inputMap) {

        int inputIndex = std::get<0>(mapping);
        //std::cout << "Input index: " << inputIndex << "\n";

        std::tuple<int, int> neuronIndices = getNeuronInputIndex(inputLayer, mapping);

        int neuronIndex = std::get<0>(neuronIndices);

        //std::cout << "Neuron index: " << neuronIndex << "\n";

        int neuronInputIndex = std::get<1>(neuronIndices);

        //std::cout << "Neuron synapse index: " << neuronInputIndex << "\n";

        int weight = std::get<2>(mapping);
        bool* ptr = &(inputs[inputIndex]);

        //std::cout << "Setting input \n";
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

        //std::cout << "Firing spike at time:  " << time << " on input " << spike << " \n";
        inputs[spike] = true;
    }
}

void resetSpikes(bool inputs[], int numInputs)
{
    for (size_t i = 0; i < numInputs; i++) {
        inputs[i] = false;
    }
}

std::vector<std::tuple<int, int>> createSpikesFromEncoding(std::vector<int> encoding)
{
    std::vector<std::tuple<int, int>> res;
    for (int i = 0; i < encoding.size(); ++i) {
        res.push_back(std::make_tuple(encoding[i], 0));
    }

    return res;
}
