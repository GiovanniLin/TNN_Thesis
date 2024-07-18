#include "Environment.h"



void Environment::testPrint()
{
	std::cout << "Hi from Environment \n\n";
}

State Environment::getState()
{
	return state;
}