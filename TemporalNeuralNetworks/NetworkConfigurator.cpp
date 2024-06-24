#include "NetworkConfigurator.h"

NetworkConfigurator::NetworkConfigurator(std::string networkConfig) : networkConfig_(FileReader::FileReader(networkConfig))
{
    //First line of the network_config.txt must be "Network Config:"
    std::string firstLine = networkConfig_.readNextLine();
    if (firstLine.empty() || firstLine != "Network Config:") {
        throw std::runtime_error("Network configuration failed, file passed to NetworkConfigurator is not a network configuration. Make sure the first line of the text file says 'Network Config:'");
    }

    while (networkConfig_.isNextLine()) {
        std::vector<std::string> nextLine = networkConfig_.readNextLineSplit(" ");
        if (!nextLine.empty()) {
            configHandler(nextLine);
        }
        if (numInputs != -1 && numLayers != -1 && ifType != -1 && ifThreshold != -1 && fullConfigure != -1) {
            break;
        }
    }

    if (numInputs == -1 || numLayers == -1 || ifType == -1 || ifThreshold == -1 || fullConfigure == -1) {
        throw std::runtime_error("Network configuration failed, one of four arguments (IntegrateFireType, Layers, Inputs, FullConfigure) not specified");
    }
}

int NetworkConfigurator::getNumInputs() {
    return numInputs;
}

void NetworkConfigurator::setNumInputs(int numInputs)
{
    this->numInputs = numInputs;
}

int NetworkConfigurator::getNumLayers() {
    return numLayers;
}

void NetworkConfigurator::setNumLayers(int numLayers)
{
    this->numLayers = numLayers;
}

int NetworkConfigurator::getIFType() 
{
    return ifType;
}

void NetworkConfigurator::setIFType(int ifType)
{
    this->ifType = ifType;
}

int NetworkConfigurator::getIFThreshold()
{
    return ifThreshold;
}

void NetworkConfigurator::setIFThreshold(int ifThreshold)
{
    this->ifThreshold = ifThreshold;
}

void NetworkConfigurator::setFullConfigure(int fullConfigure)
{
    this->fullConfigure = fullConfigure;
}

std::vector<Layer> NetworkConfigurator::createLayers()
{
    std::vector<Layer> res;

    while (networkConfig_.isNextLine()) {
        std::vector<std::string> nextLine = networkConfig_.readNextLineSplit(" ");
        if (!nextLine.empty()) {
            Layer v = layerHandler(nextLine);
            if (!v.neurons.empty()) {
                res.push_back(v);
            }
        }
    }
    if (res.size() > numLayers) {
        throw std::runtime_error("Network configuration failed, layers not set or too many layers");
    }

    if (res.empty()) {
        throw std::runtime_error("Network configuration failed, couldn't make any layers, check 'network_config.txt'");
    }

    return res;
}

void NetworkConfigurator::configHandler(std::vector<std::string> v)
{
    if (v[0] == "IntegrateFireType:") {
        setIFType(std::stoi(v[1]));
    }
    else if (v[0] == "IntegrateFireThreshold:") {
        setIFThreshold(std::stoi(v[1]));
    }
    else if (v[0] == "Layers:") {
        setNumLayers(std::stoi(v[1]));
    }
    else if (v[0] == "Inputs:") {
        setNumInputs(std::stoi(v[1]));
    }
    else if (v[0] == "FullConfigure:") {
        if (v[1] == "Yes" || v[1] == "yes") {
            setFullConfigure(1);
        }
        else if (v[1] == "No" || v[1] == "no") {
            setFullConfigure(0);
        }
    }
    else {
        return; // do nothing
    }
}

Layer NetworkConfigurator::layerHandler(std::vector<std::string> v)
{
    Layer res;
    if (fullConfigure == 1) {
        if (v[0] == "Layer") {
            if (std::stoi(v[1]) >= numLayers) {
                throw std::runtime_error("Network configuration failed, layers not set or too many layers");
            }
            else {
                while (networkConfig_.isNextLine()) {
                    std::vector<std::string> nextLine = networkConfig_.readNextLineSplit(" ");
                    if (!nextLine.empty()) {
                        if (nextLine[0] == "End") {
                            break;
                        }
                        else if (nextLine[0] == "Neuron") {
                            if (neuronCounter != std::stoi(nextLine[1])) {
                                throw std::runtime_error("Network configuration failed, incorrect numbering of neurons \n");
                            }

                            if (nextLine[2] != "inputs:") {
                                throw std::runtime_error("Network configuration failed, invalid formatting of neuron. Correct formatting is 'Neuron # inputs: #, threshold: #' \n");
                            }

                            int neuronNumInputs = std::stoi(nextLine[3]);


                            if (nextLine[4] != "threshold:") {
                                throw std::runtime_error("Network configuration failed, invalid formatting of neuron. Correct formatting is 'Neuron # inputs: #, threshold: #' \n");
                            }

                            int neuronThreshold = std::stoi(nextLine[5]);

                            Neuron toAdd = Neuron(neuronNumInputs, neuronThreshold, ifType);

                            if (ifThreshold > 0) {
                                toAdd.setIFThreshold(ifThreshold);
                            }

                            res.addNeuron(toAdd);
                            neuronCounter += 1;
                        }
                        else {
                            throw std::runtime_error("Network configuration failed, invalid argument after 'Layer #:'. Correct arguments are 'Neuron # inputs: #, threshold: #' or 'End'");
                        }
                    }
                }
            }
        }
        else {
            // Do Nothing
        }
    }
    else if (fullConfigure == 0) {
        int numNeurons = -1;
        int layerThreshold = -1;
        int currentLayer = -1;
        if (v[0] == "Layer") {
            if (std::stoi(v[1]) >= numLayers) {
                throw std::runtime_error("Network configuration failed, layers not set or too many layers");
            }
            else {
                currentLayer = std::stoi(v[1]);
                while (networkConfig_.isNextLine()) {
                    std::vector<std::string> nextLine = networkConfig_.readNextLineSplit(" ");
                    if (!nextLine.empty()) {
                        if (nextLine[0] == "End") {
                            break;
                        }
                        else if (nextLine[0] == "Neurons:") {
                            numNeurons = std::stoi(nextLine[1]);
                        }
                        else if (nextLine[0] == "Threshold:") {
                            layerThreshold = std::stoi(nextLine[1]);
                        }
                        else {
                            throw std::runtime_error("Network configuration failed, invalid argument after 'Layer #:'. Correct arguments are 'Neurons: #', 'Threshold: #' or 'End'");
                        }
                    }
                }

                if (numNeurons < 0 || layerThreshold < 0) {
                    throw std::runtime_error("Network configuration failed, invalid value specified for 'Neurons: #' or 'Threshold: #'");
                }

                for (int i = 0; i < numNeurons; ++i) {
                    if (currentLayer == 0) {
                        Neuron toAdd = Neuron(numInputs, layerThreshold, ifType);

                        if (ifThreshold > 0) {
                            toAdd.setIFThreshold(ifThreshold);
                        }

                        res.addNeuron(toAdd);
                    }
                    else {
                        Neuron toAdd = Neuron(numNeuronsPrevLayer, layerThreshold, ifType);

                        if (ifThreshold > 0) {
                            toAdd.setIFThreshold(ifThreshold);
                        }

                        res.addNeuron(toAdd);
                    }
                }

            }
        }
        else {
            // Do Nothing
        }
    }
    else {
        throw std::runtime_error("Network configuration failed, FullConfigure was not set. Please make sure FullConfigure is set to Yes or No");
    }
    neuronCounter = 0;
    numNeuronsPrevLayer = res.neurons.size();
    return res;
}

NetworkConfigurator::~NetworkConfigurator()
{
    networkConfig_.closeFile();
    std::cout << "Closing network configurator. \n";
}
