#pragma once
#include <cmath>
#include <iostream>
#include <random>
#define _USE_MATH_DEFINES
#include <math.h>

class State
{
public:
	const double gravity = 9.8;
	const double massCart = 0.711;
	const double massPole = 0.209;
	const double totalMass = massCart + massPole;
	const double length = 0.326;
	const double poleML = massPole * length;
	const double forceMagnitude = 10.0;
	const double tau = 0.01; // Timestep
	const double angleThreshold = 12.0 * (M_PI / 180);
	const double displacementThreshold = 2.4;
	const double randLowerBound = -2;
	const double randUpperBound = 2;

	State();
	double getDisplacement() const;
	void setDisplacement(double x);
	double getDisplacementDot() const;
	void setDisplacementDot(double xDot);
	double getDisplacementDotDot() const;
	double getAngle() const;
	double getAngleRad() const;
	void setAngle(double theta);
	double getAngleDot() const;
	void setAngleDot(double thetaDot);
	double getAngleDotDot() const;
	double getAnglePrev() const;
	bool step(bool action);
	void resetState(bool random);
	double randomAngle();
	const int GetSeed() const { return seed; }
private:
	const int seed = 0; // 0 to 7
	const bool euler = false;
	std::uniform_real_distribution<double> unif;
	std::default_random_engine re;
	double displacement = 0; // Displacement of cart
	double displacementDot = 0; // Derivative of displacement, velocity of cart
	double displacementDotDot = 0;
	double angle = 0; // Angle of pole
	double angleDot = 0; // Derivative of pole angle, velocity of pole
	double angleDotDot = 0;
	double anglePrev = 0;
};

