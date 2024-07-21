#pragma once
#include <cmath>
#include <iostream>
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
	double getDisplacement();
	void setDisplacement(double x);
	double getDisplacementDot();
	void setDisplacementDot(double xDot);
	double getAngle();
	void setAngle(double theta);
	double getAngleDot();
	void setAngleDot(double thetaDot);
	double getAnglePrev();
	bool step(bool action);
private:
	double displacement = 0; // Displacement of cart
	double displacementDot = 0; // Derivative of displacement, velocity of cart
	double angle = 0; // Angle of pole
	double angleDot = 0; // Derivative of pole angle, velocity of pole
	double anglePrev = 0;
};

