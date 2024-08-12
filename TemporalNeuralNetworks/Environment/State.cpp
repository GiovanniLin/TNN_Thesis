#include "State.h"

State::State()
{
	this->unif = std::uniform_real_distribution<double>(randLowerBound, randUpperBound);
	this->re = std::default_random_engine(seed);
}

double State::getDisplacement() const
{
	return displacement;
}

void State::setDisplacement(double x)
{
	this->displacement = x;
}

double State::getDisplacementDot() const
{
	return displacementDot;
}

void State::setDisplacementDot(double xDot)
{
	this->displacementDot = xDot;
}

double State::getDisplacementDotDot() const
{
	return displacementDotDot;
}

double State::getAngle() const
{
	return angle * (180 / M_PI);
}

double State::getAngleRad() const
{
	return angle;
}

void State::setAngle(double theta)
{
	this->angle = theta;
}

double State::getAngleDot() const
{
	return angleDot;
}

void State::setAngleDot(double thetaDot)
{
	this->angleDot = thetaDot;
}

double State::getAngleDotDot() const
{
	return angleDotDot;
}

double State::getAnglePrev() const
{
	return anglePrev;
}

// action true is positive force, action false is negative force
// returns true if state is not within thresholds
bool State::step(bool action)
{
	double x = getDisplacement();
	double xDot = getDisplacementDot();
	double theta = getAngleRad();
	double thetaDot = getAngleDot();
	
	double force = forceMagnitude;
	if (!action) {
		force = -forceMagnitude;
	}

	double costheta = std::cos(theta);
	double sintheta = std::sin(theta);

	double temp = (force + poleML * (thetaDot * thetaDot) * sintheta) / totalMass;
	double thetaAcc = (gravity * sintheta - costheta * temp) / (length * ((4.0 / 3.0) - massPole * (costheta * costheta) / totalMass));
	double xAcc = temp - poleML * thetaAcc * costheta / totalMass;

	angleDotDot = thetaAcc;
	displacementDotDot = xAcc;

	if (euler) {
		x += tau * xDot;
		xDot += tau * xAcc;
		theta += tau * thetaDot;
		thetaDot += tau * thetaAcc;
	}
	else {
		xDot = xDot + tau * xAcc;
		x = x + tau * xDot;
		thetaDot = thetaDot + tau * thetaAcc;
		theta = theta + tau * thetaDot;

	}

	anglePrev = angle;
	setDisplacement(x);
	setDisplacementDot(xDot);
	setAngle(theta);
	setAngleDot(thetaDot);

	bool res = (std::abs(x) > displacementThreshold || std::abs(theta) > angleThreshold);

	if (res) {
		std::cout << "Sim out of bounds \n";
		std::cout << "Pole angle: " << theta << ", pole angle threshold: " << angleThreshold << " \n";
		std::cout << "Cart displacement: " << x << ", cart displacement threshold: " << displacementThreshold << " \n\n";
	}

	return res;
}

void State::resetState(bool random)
{
	setDisplacement(0.0);
	setDisplacementDot(0.0);
	if (random) {
		setAngle(randomAngle() * 2.0 * M_PI / 360.0);
	}
	else
	{
		setAngle(0.0);
	}
	setAngleDot(0.0);
}

double State::randomAngle()
{
	return unif(re);
}