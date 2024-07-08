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
	double getRewardP();
	void setRewardP(double rewardP);
	double getRewardD();
	void setRewardD(double rewardD);
	int getRewardW();
	void setRewardW(int rewardW);
	double getPunishmentP();
	void setPunishmentP(double punishmentP);
	double getPunishmentD();
	void setPunishmentD(double punishmentD);
	int getPunishmentW();
	void setPunishmentW(int punishmentW);
	void configHandler(std::vector<std::string> v);
	void configHandlerCTNN();
	void configHandlerRTNN();
private:
	double capture = -1;
	double backoff = -1;
	double search = -1;
	double rewardP = -1; // P = Potentiation
	double rewardD = -1; // D = Depression
	int rewardW = -1; // W = Window
	double punishmentP = -1; // P = Potentiation
	double punishmentD = -1; // D = Depression
	int punishmentW = -1; // W = Window
	FileReader stdpConfig_;
};

