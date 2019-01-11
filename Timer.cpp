#include "Timer.h"


Timer::Timer()
{
}


Timer::~Timer()
{
}

void Timer::updateTime(float deltaTime)
{
    timeSinceLastRender += deltaTime;
    totalGameTime += deltaTime;
}

float Timer::getDeltaTime() const
{
    return timeSinceLastRender;
}

bool Timer::canStartNextFrame() const
{
    return timeSinceLastRender >= minTimeToNextFrame;
}

void Timer::clearTimeSinceLastRender()
{
    timeSinceLastRender = 0.0f;
}
