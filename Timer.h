#pragma once
class Timer {
public:
    Timer();
    ~Timer();

    void updateTime(float deltaTime);
    bool canStartNextFrame() const;
    float getDeltaTime() const;
    void clearTimeSinceLastRender();

protected:
    float totalGameTime{ 0.0f };
    float timeSinceLastRender{ 0.0f };

    const float minTimeToNextFrame{ 10.0f / 1000.0f };
};

