#pragma once
#include "RedBall.h"
#include "YellowBall.h"
#include "Game.h"

#include <d3dx9.h>
#include <limits>

class RedBallAI {
public:
    RedBallAI(RedBall* _actor);
    ~RedBallAI();

    D3DXVECTOR3 OnUpdate(float deltaTime);
    void AddEnergy(float inc);
    void FindNewTargetBall();

protected:
    float energy{ Game::GetInstance()->blackboard->startRedBallEnergy };
    float distanceToTarget{ (std::numeric_limits<float>::max)() };
    std::shared_ptr<YellowBall> target;

    RedBall* actor;

    friend class RedBall;
};

