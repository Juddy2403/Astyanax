#include "pch.h"
#include "GlobalFunctions.h"

bool Delay(const float delayTime, float& accumTime, float elapsedSec)
{
	accumTime += elapsedSec;
	if (accumTime >= delayTime)	
	{
		accumTime = 0;
		return true;
	}
	return false;
}