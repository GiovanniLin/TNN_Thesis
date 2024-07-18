#pragma once
#include <vector>
#include <numeric>
#include <iostream>
#include <memory>

class IntegrateFire
{
public:
	static std::shared_ptr<IntegrateFire> chooseIntegrateFire(int type, int numInputs);
	virtual int getIFThreshold() {
		std::cout << "Wrong getIFThreshold \n";
		return -1;
	}
	virtual void setIFThreshold(int ifThreshold) {
		std::cout << "Wrong setIFThreshold \n";
		// does nothing
	}
	virtual void accumulatePotential(int input, int weight) {
		std::cout << "Wrong accumulatePotential \n";
		// does nothing
	};
	virtual bool checkThreshold(int threshold) {
		std::cout << "Wrong checkThreshold \n";
		return false;
	};
	virtual int currentBodyPotential() {
		std::cout << "Wrong currentBodyPotential \n";
		return 0;
	};
	virtual std::string toString() {
		return "IntegrateFire";
	}
	virtual void setSpikeFlag(int index) {
		std::cout << "Wrong setSpikeFlag \n";
		// does nothing
	};
	virtual void resetAllSpikeFlag() {
		std::cout << "Wrong resetSpikeFlag \n";
		// does nothing
	}
	virtual bool getSpikeFlag(int index) {
		std::cout << "Wrong getSpikeFlag \n";
		return false;
	};
};

class RampIntegrateFire : public IntegrateFire
{
private:
	std::vector<int> rifCounter; // Keeps track of amplitude of RIF on input i
	std::vector<bool> spikeFlag; // If true, do RIF on input I. Only becomes true when spike is detected.
	int rifThreshold = -1;
public:
	RampIntegrateFire(int numInputs);
	int getIFThreshold() override;
	void setIFThreshold(int ifThreshold) override;
	void accumulatePotential(int input, int weight) override;
	bool checkThreshold(int threshold) override;
	int currentBodyPotential() override;
	std::string toString() override {
		return "RampIntegrateFire";
	}
	void setSpikeFlag(int index) override;
	void resetAllSpikeFlag() override;
	bool getSpikeFlag(int index) override;
};

class LeakyIntegrateFire : public IntegrateFire
{
public:
	// Add later when needed
};
