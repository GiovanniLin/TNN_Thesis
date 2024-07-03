#pragma once
#include "FileReader.h"
#include <iostream>
#include <vector>

class STDPConfigurator
{
public:
	STDPConfigurator(std::string stdpConfig);
	double getCapture();
	void setCapture(double capture);
	double getBackoff();
	void setBackoff(double backoff);
	double getSearch();
	void setSearch(double search);
	double getRewardPD();
	void setRewardPD(double rewardPD);
	int getRewardW();
	void setRewardW(int rewardW);
	double getPunishmentPD();
	void setPunishmentPD(double punishmentPD);
	int getPunishmentW();
	void setPunishmentW(int punishmentW);
	void configHandler(std::vector<std::string> v);
	void configHandlerCTNN();
	void configHandlerRTNN();
private:
	double capture = -1;
	double backoff = -1;
	double search = -1;
	double rewardPD = -1; // PD = Potentiation and Depression
	int rewardW = -1; // W = Window
	double punishmentPD = -1; // PD = Potentiation and Depression
	int punishmentW = -1; // W = Window
	FileReader stdpConfig_;
};

