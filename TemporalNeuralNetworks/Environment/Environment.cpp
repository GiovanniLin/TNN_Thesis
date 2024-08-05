#include "Environment.h"

void Environment::testPrint()
{
	std::cout << "Environment has been initialized. \n\n";
}

State Environment::getState()
{
	return state;
}

void Environment::resetState(bool random) {
	state.resetState(random);
}

bool Environment::decode(int output)
{
	if (output == 0) {
		return true;
	}
	else {
		return false;
	}
}

void Environment::printState()
{
	std::cout << "Angle of pole: " << state.getAngle() << " \n";
	std::cout << "Velocity of pole: " << state.getAngleDot() << " \n";
	std::cout << "Displacement of cart: " << state.getDisplacement() << " \n";
	std::cout << "Velocity of cart: " << state.getDisplacementDot() << " \n\n";
}

int Environment::determineReward(int cycleCounter, int episodeCounter)
{
	if (episodeCounter >= addRewardAfter) {
		if (cycleCounter % rewardCycleThreshold == 0 && cycleCounter != 0) {
			return 1;
		}
		else if (std::abs(state.getAngleRad()) > state.angleThreshold) {
			return -1;
		}
		else {
			return 0;
		}
	}
	else {
		return 0;
	}
}

bool Environment::stepState(bool action)
{
	return state.step(action);
}

void Environment::testMath()
{
	double x = state.getDisplacement();
	double xDot = state.getDisplacementDot();
	double theta = state.getAngleRad();
	double thetaDot = state.getAngleDot();
	double force = state.forceMagnitude;

	//force = -force;

	double costheta = std::cos(theta);
	double sintheta = std::sin(theta);

	double temp = (force + state.poleML * (thetaDot * thetaDot) * sintheta) / state.totalMass;
	double thetaAcc = (state.gravity * sintheta - costheta * temp) / (state.length * (4.0 / 3.0 - state.massPole * (costheta * costheta) / state.totalMass));
	double xAcc = temp - state.poleML * thetaAcc * costheta / state.totalMass;

	x = x + state.tau * xDot;
	xDot = xDot + state.tau * xAcc;
	theta = theta + state.tau * thetaDot;
	thetaDot = thetaDot + state.tau * thetaAcc;

	std::cout << "Pole Angle after 1 cycle: " << (theta / (2 * pi()) * 360) << "\n";
	std::cout << "Pole Velocity after 1 cycle: " << std::fixed << std::setprecision(std::numeric_limits<double>::max_digits10) << thetaDot << "\n";
	std::cout << "Pole acceleration in 1 cycle: " << thetaAcc << "\n";
	std::cout << "Cart Displacement after 1 cycle: " << x << "\n";
	std::cout << "Cart Velocity after 1 cycle: " << xDot << "\n";
	std::cout << "Cart acceleration in 1 cycle: " << xAcc << "\n";

	//force = -force;

	costheta = std::cos(theta);
	sintheta = std::sin(theta);

	temp = (force + state.poleML * (thetaDot * thetaDot) * sintheta) / state.totalMass;
	thetaAcc = (state.gravity * sintheta - costheta * temp) / (state.length * (4.0 / 3.0 - state.massPole * (costheta * costheta) / state.totalMass));
	xAcc = temp - state.poleML * thetaAcc * costheta / state.totalMass;

	x = x + state.tau * xDot;
	xDot = xDot + state.tau * xAcc;
	theta = theta + state.tau * thetaDot;
	thetaDot = thetaDot + state.tau * thetaAcc;

	std::cout << "Pole Angle after 2 cycles: " << (theta / (2 * pi()) * 360) << "\n";
	std::cout << "Pole Velocity after 2 cycles: " << thetaDot << "\n";
	std::cout << "Pole acceleration in 2 cycles: " << thetaAcc << "\n";
	std::cout << "Cart Displacement after 2 cycles: " << x << "\n";
	std::cout << "Cart Velocity after 2 cycles: " << xDot << "\n";
	std::cout << "Cart acceleration in 2 cycles: " << xAcc << "\n";

	force = -force;

	costheta = std::cos(theta);
	sintheta = std::sin(theta);

	temp = (force + state.poleML * (thetaDot * thetaDot) * sintheta) / state.totalMass;
	thetaAcc = (state.gravity * sintheta - costheta * temp) / (state.length * (4.0 / 3.0 - state.massPole * (costheta * costheta) / state.totalMass));
	xAcc = temp - state.poleML * thetaAcc * costheta / state.totalMass;

	x = x + state.tau * xDot;
	xDot = xDot + state.tau * xAcc;
	theta = theta + state.tau * thetaDot;
	thetaDot = thetaDot + state.tau * thetaAcc;

	std::cout << "Pole Angle after 3 cycles: " << (theta / (2 * pi()) * 360) << "\n";
	std::cout << "Pole Velocity after 3 cycles: " << thetaDot << "\n";
	std::cout << "Pole acceleration in 3 cycles: " << thetaAcc << "\n";
	std::cout << "Cart Displacement after 3 cycles: " << x << "\n";
	std::cout << "Cart Velocity after 3 cycles: " << xDot << "\n";
	std::cout << "Cart acceleration in 3 cycles: " << xAcc << "\n";
	std::cout << "\n";
}