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

// action true is positive force, action false is negative force
// returns false if state is not within thresholds
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

	//temp = (force + self.polemass_length * theta_dot**2 * sintheta) / self.total_mass
	//thetaacc = (self.gravity * sintheta - costheta * temp) / (self.length * (4.0 / 3.0 - self.masspole * costheta * *2 / self.total_mass))
	//xacc = temp - self.polemass_length * thetaacc * costheta / self.total_mass
	double temp = (force + poleML * (thetaDot * thetaDot) * sintheta) / totalMass;
	double thetaAcc = (gravity * sintheta - costheta * temp) / (length * (4.0 / 3.0 - massPole * (costheta * costheta) / totalMass));
	double xAcc = temp - poleML * thetaAcc * costheta / totalMass;

	// Ask about order of operations
	x = x + tau * xDot;
	xDot = xDot + tau * xAcc;
	theta = theta + tau * thetaDot;
	thetaDot = thetaDot + tau * thetaAcc;

	setDisplacement(x);
	setDisplacementDot(xDot);
	setAngle(theta);
	setAngleDot(thetaDot);

	return (x < -displacementThreshold || x > displacementThreshold || theta < -angleThreshold || theta > angleThreshold);
}