#include "State.h"

State::State()
{
	// does nothing, except for create a State
}

double State::getDisplacement()
{
	return displacement;
}

void State::setDisplacement(double x)
{
	this->displacement = x;
}

double State::getDisplacementDot()
{
	return displacementDot;
}

void State::setDisplacementDot(double xDot)
{
	this->displacementDot = xDot;
}

double State::getAngle()
{
	return angle;
}

void State::setAngle(double theta)
{
	this->angle = theta;
}

double State::getAngleDot()
{
	return angleDot;
}

void State::setAngleDot(double thetaDot)
{
	this->angleDot = thetaDot;
}

double State::getAnglePrev()
{
	return anglePrev;
}

// action true is positive force, action false is negative force
// returns true if state is not within thresholds
bool State::step(bool action)
{
	double x = getDisplacement();
	double xDot = getDisplacementDot();
	double theta = getAngle();
	double thetaDot = getAngleDot();
	
	double force = forceMagnitude;
	if (!action) {
		force = -forceMagnitude;
	}

	double costheta = std::cos(theta);
	double sintheta = std::sin(theta);

	double temp = (force + poleML * (thetaDot * thetaDot) * sintheta) / totalMass;
	double thetaAcc = (gravity * sintheta - costheta * temp) / (length * (4.0 / 3.0 - massPole * (costheta * costheta) / totalMass));
	double xAcc = temp - poleML * thetaAcc * costheta / totalMass;

	x = x + tau * xDot;
	xDot = xDot + tau * xAcc;
	theta = theta + tau * thetaDot;
	thetaDot = thetaDot + tau * thetaAcc;

	anglePrev = angle;
	setDisplacement(x);
	setDisplacementDot(xDot);
	setAngle(theta);
	setAngleDot(thetaDot);

	return (x < -displacementThreshold || x > displacementThreshold || theta < -angleThreshold || theta > angleThreshold);
}