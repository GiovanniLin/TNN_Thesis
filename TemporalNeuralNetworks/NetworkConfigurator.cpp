#include "NetworkConfigurator.h"

NetworkConfigurator::NetworkConfigurator(FileReader& networkConfig)
{
    //First line of the network_config.txt must be "Network Config:"
    std::string firstLine = networkConfig.readNextLine();
    if (firstLine.empty() || firstLine != "Network Config:") {
        throw std::runtime_error("Network configuration failed, file passed to NetworkConfigurator is not a network configuration. Make sure the first line of the text file says 'Network Config:'");
    }

    while (networkConfig.isNextLine()) {
        std::vector<std::string> nextLine = networkConfig.readNextLineSplit(" ");
        if (!nextLine.empty()) {
            configHandler(nextLine);
        }
        if (numInputs != -1 && numLayers != -1 && ifType != -1) {
            break;
        }
    }

    if (numInputs == -1 || numLayers == -1 || ifType == -1) {
        throw std::runtime_error("Network configuration failed, one of three arguments not specified");
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

int NetworkConfigurator::getIFType() {
    return ifType;
}

void NetworkConfigurator::setIFType(int ifType)
{
    this->ifType = ifType;
}

std::vector<std::vector<Neuron>> NetworkConfigurator::createLayers(FileReader& networkConfig)
{
    std::vector<std::vector<Neuron>> res;
    
    //int counter = 0;

    while (networkConfig.isNextLine()) {
        std::vector<std::string> nextLine = networkConfig.readNextLineSplit(" ");
        if (!nextLine.empty()) {
            //std::cout << "Creating layer " << counter << " \n";
            std::vector<Neuron> v = layerHandler(nextLine, networkConfig);
            if (!v.empty()) {
                res.push_back(v);
                //counter++;
            }
        }
    }
    if (res.size() > numLayers) {
        throw std::runtime_error("Network configuration failed, layers not set or too many layers");
    }
    return res;
}

void NetworkConfigurator::configHandler(std::vector<std::string> v)
{
    if (v[0] == "IntegrateFireType:") {
        setIFType(std::stoi(v[1]));
    }
    else if (v[0] == "Layers:") {
        setNumLayers(std::stoi(v[1]));
    }
    else if (v[0] == "Inputs:") {
        setNumInputs(std::stoi(v[1]));
    }
    else {
        return; // do nothing
    }
}

std::vector<Neuron> NetworkConfigurator::layerHandler(std::vector<std::string> v, FileReader& networkConfig)
{
    std::vector<Neuron> res;
    if (v[0] == "Layer") {
        if (std::stoi(v[1]) >= numLayers) {
            throw std::runtime_error("Network configuration failed, layers not set or too many layers");
        }
        else {
            while (networkConfig.isNextLine()) {
                int counter = 0;
                std::vector<std::string> nextLine = networkConfig.readNextLineSplit(" ");
                if (!nextLine.empty()) {
                    if (nextLine[0] == "End") {
                        break;
                    }
                    else if (nextLine[0] == "Neuron") {
                        //std::cout << "Found keyword '" << nextLine[0] << "' \n";
                        if (counter != std::stoi(nextLine[1])) {
                            throw std::runtime_error("Network configuration failed, incorrect numbering of neurons");
                        }

                        //std::cout << "Numbering of Neurons correct \n";
                        if (nextLine[2] != "inputs:") {
                            throw std::runtime_error("Network configuration failed, invalid formatting of neuron. Correct formatting is 'Neuron # inputs: #, threshold: #'");
                        }

                        //std::cout << "Found keyword '" << nextLine[2] << "' \n";
                        int neuronNumInputs = std::stoi(nextLine[3]);

                        //std::cout << "Neuron number of inputs set \n";

                        if (nextLine[4] != "threshold:") {
                            throw std::runtime_error("Network configuration failed, invalid formatting of neuron. Correct formatting is 'Neuron # inputs: #, threshold: #'");
                        }

                        //std::cout << "Found keyword'" << nextLine[4] << "' \n";
                        int neuronThreshold = std::stoi(nextLine[5]);

                        //std::cout << "Neuron threshold set \n";
                        res.push_back(Neuron(neuronNumInputs, neuronThreshold, ifType));
                        counter += 1;
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
    return res;
}