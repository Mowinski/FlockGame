#include "Timer.h"

void Timer::updateTime(float deltaTime)
{
    timeSinceLastRender += deltaTime;
    totalGameTime += deltaTime;
}

float Timer::getRoundTime() const
{
	if (roundIsFreezed) { return roundTime; }
	return totalGameTime;
}

void Timer::freezeRoundTime()
{
	roundTime = totalGameTime;
	roundIsFreezed = true;
}
