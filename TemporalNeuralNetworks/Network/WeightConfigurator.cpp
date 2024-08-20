#include "WeightConfigurator.h"

WeightConfigurator::WeightConfigurator(std::string weightConfig) : weightConfig_(FileReader::FileReader(weightConfig))
{
    //First line of the weight_config.txt must be "Weight Config:"
    std::string firstLine = weightConfig_.readNextLine();
    if (firstLine.empty() || firstLine != "Weight Config:") {
        throw std::runtime_error("Weight configuration failed, file passed to WeightConfigurator is not a weight configuration. Make sure the first line of the text file says 'Weight Config:'");
    }
}

std::vector<std::vector<std::vector<double>>> WeightConfigurator::getWeights()
{
    return weights;
}

void WeightConfigurator::readWeights(std::vector<Layer>& layers)
{
    int layerCount = 0;
    while (weightConfig_.isNextLine()) {
        std::vector<std::string> nextLine = weightConfig_.readNextLineSplit(" ");
        if (!nextLine.empty()) {
            if (nextLine[0] == "Layer") {
                std::vector<std::vector<double>> resLayer;
                if (std::stoi(nextLine[1]) != layerCount) {
                    throw std::runtime_error("Reading weights failed, formatting of file is incorrect, number in 'Layer #:' out of order.");
                }
                int neuronCount = 0;
                while (weightConfig_.isNextLine()) {
                    std::vector<std::string> nL = weightConfig_.readNextLineSplit(" ");
                    std::vector<double> resNeuron;
                    if (!nL.empty()) {
                        if (nL[0] == "End") {
                            break;
                        }
                        if (nL[0] == "Neuron") {
                            if (std::stoi(nL[1]) != neuronCount) {
                                throw std::runtime_error("Reading weights failed, formatting of file is incorrect, number in 'Neuron # Start' out of order.");
                            }
                            if (nL[2] != "Start") {
                                throw std::runtime_error("Reading weights failed, formatting of file is incorrect, expected 'Start' after 'Neuron #'.");
                            }
                            int synapseCount = 0;
                            while (weightConfig_.isNextLine()) {
                                std::vector<std::string> v = weightConfig_.readNextLineSplit(" ");
                                if (!v.empty()) {
                                    if (v[0] == "Neuron" && std::stoi(v[1]) == neuronCount && v[2] == "End") {
                                        neuronCount += 1;
                                        break;
                                    }
                                    if (v[0] == "Input") {
                                        if (std::stoi(v[1]) != synapseCount) {
                                            throw std::runtime_error("Reading weights failed, formatting of file is incorrect, number in 'Input #: (weight)' out of order.");
                                        }
                                        if (v.size() < 2) {
                                            throw std::runtime_error("Reading weights failed, formatting of file is incorrect, missing argument after in 'Input #:'.");
                                        }
                                        resNeuron.push_back(std::stod(v[2]));
                                    }
                                    else {
                                        throw std::runtime_error("Reading weights failed, formatting of file is incorrect, expected 'Input #: (weight)' or 'Neuron # End'.");
                                    }
                                    synapseCount += 1;
                                }
                            }
                        }
                        else {
                            throw std::runtime_error("Reading weights failed, formatting of file is incorrect, expected 'Neuron # Start'.");
                        }
                    }
                    resLayer.push_back(resNeuron);
                }
                weights.push_back(resLayer);
                layerCount += 1;
            }
            else {
                throw std::runtime_error("Reading weights failed, formatting of file is incorrect, expected 'Layer #:'");
            }
        }
    }

    if (layers.size() != weights.size()) {
        throw std::runtime_error("Reading weights failed, number of layers in weights is not equal to number of layers in network");
    }

    for (size_t i = 0; i < layers.size(); ++i) {
        if (!weights[i].empty()) {
            if (layers[i].getNeurons().size() != weights[i].size()) {
                throw std::runtime_error("Reading weights failed, number of neurons in weights is not equal to number of neurons in network");
            }
        }
    }
}