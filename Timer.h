#pragma once
class Timer {
public:
    Timer() = default;
    ~Timer() = default;

	float getDeltaTime() const;
	float getRoundTime() const;
	void freezeRoundTime();
    void updateTime(float deltaTime);
    bool canStartNextFrame() const;
    void clearTimeSinceLastRender();

protected:
    float totalGameTime{ 0.0f };
    float timeSinceLastRender{ 0.0f };
	float roundTime{ 0.0f };
	bool roundIsFreezed{ false };

    const float minTimeToNextFrame{ 10.0f / 1000.0f };
};

