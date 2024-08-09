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
        if (numInputs != -1 && numLayers != -1 && ifType != -1 && ifThreshold != -1 && fullConfigure != -1 && !intervals.empty() && mHotCode != -1) {
            break;
        }
    }

    if (numInputs == -1 || numLayers == -1 || ifType == -1 || ifThreshold == -1 || fullConfigure == -1 || intervals.empty() || mHotCode == -1) {
        throw std::runtime_error("Network configuration failed, one of five arguments (IntegrateFireType, Layers, Inputs, FullConfigure, EnvironmentVariables) not specified");
    }

    createIntervals();

    for (int i = 0; i < intervals.size(); ++i) {
        if (intervals[i].empty()) {
            throw std::runtime_error("Network configuration failed, one of the encoding intervals was not set");
        }
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

std::vector<std::vector<double>> NetworkConfigurator::getIntervals()
{
    return intervals;
}

int NetworkConfigurator::getMHotCode()
{
    return mHotCode;
}

void NetworkConfigurator::setMHotCode(int mHotCode)
{
    this->mHotCode = mHotCode;
}

std::vector<Layer> NetworkConfigurator::createLayers()
{
    std::vector<Layer> res;

    while (networkConfig_.isNextLine()) {
        std::vector<std::string> nextLine = networkConfig_.readNextLineSplit(" ");
        if (!nextLine.empty()) {
            Layer v = layerHandler(nextLine);
            if (!v.getNeurons().empty()) {
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
    else if (v[0] == "EnvironmentVariables:") {
        int ev = std::stoi(v[1]);
        for (int i = 0; i < ev; ++i) {
            std::vector<double> toAdd;
            intervals.push_back(toAdd);
        }
    }
    else if (v[0] == "m-hotCode:") {
        setMHotCode(std::stoi(v[1]));
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
        int layerType = -1;
        int readWeights = -1;
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
                        else if (nextLine[0] == "Type:") {
                            layerType = std::stoi(nextLine[1]);
                        }
                        else if (nextLine[0] == "ReadWeights:") {
                            readWeights = std::stoi(nextLine[1]);
                        }
                        else {
                            throw std::runtime_error("Network configuration failed, invalid argument after 'Layer #:'. Correct arguments are 'Neurons: #', 'Threshold: #', 'Type: #', 'ReadWeights: #' or 'End'");
                        }
                    }
                }

                if (numNeurons < 0 || layerThreshold < 0 || layerType < 0 || readWeights < 0 || readWeights > 1) {
                    throw std::runtime_error("Network configuration failed, invalid value specified for 'Neurons: #', 'Threshold: #', 'Type: #' or 'ReadWeights: #'");
                }

                if (readWeights == 1) {
                    res.setReadWeights(true);
                }
                res.setTypeTNN(layerType);

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
    numNeuronsPrevLayer = static_cast<int>(res.getNeurons().size());
    return res;
}

void NetworkConfigurator::createIntervals()
{
    bool keepReading = false; 
    while (networkConfig_.isNextLine() && !keepReading) {
        std::vector<std::string> nL = networkConfig_.readNextLineSplit(" ");
        if (!nL.empty()) {
            if (nL[0] == "StartIntervals") {
                keepReading = true;
            }
            else {
                throw std::runtime_error("Network configuration failed, couldn't read encoding intervals, file out of order, check if intervals section starts with 'StartIntervals'");
            }
        }
    }

    int variableCounter = 0;

    while (networkConfig_.isNextLine() && keepReading) {
        int intervalCounter = 0;
        std::vector<std::string> nL = networkConfig_.readNextLineSplit(" ");
        if (!nL.empty()) {
            if (nL[0] == "Variable") {
                if (variableCounter >= intervals.size()) {
                    throw std::runtime_error("Network configuration failed, too many environment variables specified, check if the correct number is set at 'EnvironmentVariables: #'");
                }
                if (std::stoi(nL[1]) == variableCounter) {
                    while (networkConfig_.isNextLine()) {
                        std::vector<std::string> v = networkConfig_.readNextLineSplit(" ");
                        if (!v.empty()) {
                            if (v[0] == "End") {
                                break;
                            }
                            else if (v[0] == "Interval") {
                                if (std::stoi(v[1]) == intervalCounter) {
                                    if (v[2] == "-inf") {
                                        intervals[variableCounter].push_back(std::numeric_limits<double>::lowest());
                                    }
                                    else if (v[2] == "inf") {
                                        intervals[variableCounter].push_back(std::numeric_limits<double>::max());
                                    }
                                    else {
                                        intervals[variableCounter].push_back(std::stod(v[2]));
                                    }
                                    intervalCounter += 1;
                                }
                                else {
                                    throw std::runtime_error("Network configuration failed, couldn't read encoding intervals, numbers after 'Interval' out of order");
                                }
                            }
                            else {
                                throw std::runtime_error("Network configuration failed, couldn't read encoding intervals, file out of order, valid arguments are 'Interval #:' or 'End'");
                            }
                        }

                    }
                    variableCounter += 1;
                }
                else {
                    throw std::runtime_error("Network configuration failed, couldn't read encoding intervals, numbers after 'Variable' out of order");
                }
            }
            else if (nL[0] == "EndIntervals") {
                keepReading = false;
            }
            else {
                throw std::runtime_error("Network configuration failed, couldn't read encoding intervals, file out of order, valid arguments are 'Variable #:' or 'EndIntervals'");
            }
        }
    }
}

std::vector<int> NetworkConfigurator::getEncoding(int ev, double value)
{
    if (ev >= intervals.size()) {
        throw std::runtime_error("Get Encoding failed, Environment Variable out of bounds");
    }

    int res = 0;
    /*for (int i = 0; i < ev; ++i) {
        res += (static_cast<int>(intervals[i].size()) - 1);
    }*/

    for (int i = 0; i < intervals[ev].size() - 1; ++i) {
        if (i == intervals[ev].size() - 2) {
            if (intervals[ev][i] <= value && value <= intervals[ev][i + 1]) {
                res += i;
                //std::cout << "Angle Interval: " << intervals[ev][i] << " - " << intervals[ev][i + 1] << " \n";
                break;
            }
        }
        else {
            if (intervals[ev][i] <= value && value < intervals[ev][i + 1]) {
                res += i;
                //std::cout << "Angle Interval: " << intervals[ev][i] << " - " << intervals[ev][i + 1] << " \n";
                break;
            }
        }
    }

    std::vector<int> resV;
    for (int i = 0; i < mHotCode; ++i) {
        resV.push_back(res + i);
    }

    return resV;
}

NetworkConfigurator::~NetworkConfigurator()
{
    networkConfig_.closeFile();
    std::cout << "Closing network configurator. \n";
}
