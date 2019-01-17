#pragma once
#include "RedBall.h"
#include "YellowBall.h"
#include "Game.h"

#include <d3dx9math.h>

class RedBallAI {
public:
    RedBallAI(RedBall* _actor);
    ~RedBallAI() = default;

    D3DXVECTOR3 onUpdate(float deltaTime);
    void findNewTargetBall();
	void addEnergy(float inc) { energy += inc; };
	void unsetTarget() { target = nullptr; };

protected:
	RedBall* actor;

    float energy{ Game::getInstance()->blackboard->startRedBallEnergy };
	std::shared_ptr<YellowBall> target{ nullptr };

};

