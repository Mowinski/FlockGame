#pragma once
#include "RedBall.h"
#include "YellowBall.h"

#include <d3dx9.h>
#include <limits>

class RedBallAI
{
public:
	RedBallAI(RedBall* _actor);
	~RedBallAI();

	D3DXVECTOR3 OnUpdate(float deltaTime);

protected:
	float energy{ 100.0f };
	float distanceToTarget{ (std::numeric_limits<float>::max)() };
	std::shared_ptr<YellowBall> target;

	RedBall* actor;
};

