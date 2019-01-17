#pragma once
class Timer {
public:
    Timer() = default;
    ~Timer() = default;

	float getDeltaTime() const { return timeSinceLastRender; };
	bool canStartNextFrame() const { return timeSinceLastRender >= minTimeToNextFrame; };
	void clearTimeSinceLastRender() { timeSinceLastRender = 0.0f; };
	void updateTime(float deltaTime);
	float getRoundTime() const;
	void freezeRoundTime();


protected:
    float totalGameTime{ 0.0f };
    float timeSinceLastRender{ 0.0f };
	float roundTime{ 0.0f };
	bool roundIsFreezed{ false };

    const float minTimeToNextFrame{ 10.0f / 1000.0f };
};

