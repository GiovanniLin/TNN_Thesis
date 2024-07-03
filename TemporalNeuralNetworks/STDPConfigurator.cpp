#include "STDPConfigurator.h"

STDPConfigurator::STDPConfigurator(std::string stdpConfig) : stdpConfig_(FileReader::FileReader(stdpConfig))
{
	//First line of the network_config.txt must be "STDP Config:"
	std::string firstLine = stdpConfig_.readNextLine();
	if (firstLine.empty() || firstLine != "STDP Config:") {
		throw std::runtime_error("STDP configuration failed, file passed to STDPConfigurator is not an STDP configuration. Please make sure the first line of the file says 'STDP Config:'");
	}

	while (stdpConfig_.isNextLine()) {
		std::vector<std::string> nextLine = stdpConfig_.readNextLineSplit(" ");
		if (!nextLine.empty()) {
			configHandler(nextLine);
		}
		// if (numInputs != -1 && numLayers != -1 && ifType != -1 && ifThreshold != -1 && fullConfigure != -1) {
			// break;
		// }
		if (capture != -1 && backoff != -1 && search != -1 && rewardPD != -1 && rewardW != -1 && punishmentPD != -1 && punishmentW != -1) {
			break;
		}
	}

	if (capture == -1 || backoff == -1 || search == -1 || rewardPD == -1 || rewardW == -1 || punishmentPD == -1 || punishmentW == -1) {
		throw std::runtime_error("STDP configuration failed, one of seven STDP parameters was not set.");
	}
}

double STDPConfigurator::getCapture()
{
	return capture;
}

void  STDPConfigurator::setCapture(double capture)
{
	this->capture = capture;
}

double STDPConfigurator::getBackoff()
{
	return backoff;
}

void STDPConfigurator::setBackoff(double backoff)
{
	this->backoff = backoff;
}

double STDPConfigurator::getSearch()
{
	return search;
}

void STDPConfigurator::setSearch(double search)
{
	this->search = search;
}

double STDPConfigurator::getRewardPD()
{
	return rewardPD;
}

void STDPConfigurator::setRewardPD(double rewardPD)
{
	this->rewardPD = rewardPD;
}

int STDPConfigurator::getRewardW()
{
	return rewardW;
}

void STDPConfigurator::setRewardW(int rewardW)
{
	this->rewardW = rewardW;
}

double STDPConfigurator::getPunishmentPD()
{
	return punishmentPD;
}

void STDPConfigurator::setPunishmentPD(double punishmentPD)
{
	this->punishmentPD = punishmentPD;
}

int STDPConfigurator::getPunishmentW()
{
	return punishmentW;
}

void STDPConfigurator::setPunishmentW(int punishmentW)
{
	this->punishmentW = punishmentW;
}

void STDPConfigurator::configHandler(std::vector<std::string> v)
{
	if (v[0] == "C-TNN:") {
		configHandlerCTNN();
	}
	else if (v[0] == "R-TNN:") {
		configHandlerRTNN();
	}
	else {
		return; // do nothing
	}
}

void STDPConfigurator::configHandlerCTNN()
{
	while (stdpConfig_.isNextLine()) {
		std::vector<std::string> v = stdpConfig_.readNextLineSplit(" ");
		if (!v.empty()) {
			if (v[0] == "Capture:") {
				setCapture(std::stod(v[1]));
			}
			else if (v[0] == "Backoff:") {
				setBackoff(std::stod(v[1]));
			}
			else if (v[0] == "Search:") {
				setSearch(std::stod(v[1]));
			}
			else {
				continue;
			}
		}
		if (capture != -1 && backoff != -1 && search != -1) {
			break;
		}
	}

	if (capture == -1 || backoff == -1 || search == -1) {
		throw std::runtime_error("STDP configuration failed, one of three C-TNN parameters (Capture, Backoff, Search) was not set");
	}
}

void STDPConfigurator::configHandlerRTNN()
{
	while (stdpConfig_.isNextLine()) {
		std::vector<std::string> v = stdpConfig_.readNextLineSplit(" ");
		if (!v.empty()) {
			if (v[0] == "RewardPotentiationDepression:") {
				setRewardPD(std::stod(v[1]));
			}
			else if (v[0] == "RewardWindow:") {
				setRewardW(std::stoi(v[1]));
			}
			else if (v[0] == "PunishmentPotentiationDepression:") {
				setPunishmentPD(std::stod(v[1]));
			}
			else if (v[0] == "PunishmentWindow:") {
				setPunishmentW(std::stoi(v[1]));
			}
			else {
				continue;
			}
		}
		if (rewardPD != -1 && rewardW != -1 && punishmentPD != -1 && punishmentW != -1) {
			break;
		}
	}

	if (rewardPD == -1 || rewardW == -1 || punishmentPD == -1 || punishmentW == -1) {
		throw std::runtime_error("STDP configuration failed, one of four R-TNN parameters (RewardPotentiationDepression, RewardWindow, PunishmentPotentiationDepression, PunishmentWindow) was not set");
	}
}