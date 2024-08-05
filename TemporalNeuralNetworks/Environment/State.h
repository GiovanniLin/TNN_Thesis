#pragma once
#include <cmath>
#include <iostream>
#include <random>
#include "math.h"

class State
{
public:
	State();
	const double gravity = 9.8;
	const double massCart = 1.0;
	const double massPole = 0.1;
	const double totalMass = massCart + massPole;
	const double length = 0.5;
	const double poleML = massPole * length;
	const double forceMagnitude = 10.0;
	const double tau = 0.02; // Timestep
	const double angleThreshold = 12.0 * 2.0 * pi() / 360.0;
	const double displacementThreshold = 2.4;
	const double randLowerBound = -2;
	const double randUpperBound = 2;
	const int seed = 7; // 0 to 7
	const bool euler = false;
	std::uniform_real_distribution<double> unif;
	std::default_random_engine re;
	double getDisplacement();
	void setDisplacement(double x);
	double getDisplacementDot();
	void setDisplacementDot(double xDot);
	double getAngle();
	double getAngleRad();
	void setAngle(double theta);
	double getAngleDot();
	void setAngleDot(double thetaDot);
	double getAnglePrev();
	bool step(bool action);
	void resetState(bool random);
	double randomAngle();
private:
	double displacement = 0; // Displacement of cart
	double displacementDot = 0; // Derivative of displacement, velocity of cart
	double angle = 0; // Angle of pole
	double angleDot = 0; // Derivative of pole angle, velocity of pole
	double anglePrev = 0;
};

