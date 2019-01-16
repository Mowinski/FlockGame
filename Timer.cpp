#include "Timer.h"

void Timer::updateTime(float deltaTime)
{
    timeSinceLastRender += deltaTime;
    totalGameTime += deltaTime;
}

float Timer::getDeltaTime() const
{
    return timeSinceLastRender;
}

float Timer::getRoundTime() const
{
	if (roundIsFreezed) { return roundTime; }
	return totalGameTime;
}

bool Timer::canStartNextFrame() const
{
    return timeSinceLastRender >= minTimeToNextFrame;
}

void Timer::clearTimeSinceLastRender()
{
    timeSinceLastRender = 0.0f;
}

void Timer::freezeRoundTime()
{
	roundTime = totalGameTime;
	roundIsFreezed = true;
}
