#include "RedBallAI.h"

RedBallAI::RedBallAI(RedBall* _actor) : actor(_actor)
{
}

D3DXVECTOR3 RedBallAI::onUpdate(float deltaTime)
{
	float speedValue = D3DXVec3Length(&(actor->speed));
	if (speedValue > 15.0f) { return D3DXVECTOR3{ 0.0f, 0.0f, 0.0f }; }
	if (energy < 0.0f) { return  D3DXVECTOR3{ 0.0f, 0.0f, 0.0f }; }
	if (target == nullptr) { 
		findNewTargetBall();
		if (target == nullptr) {
			return D3DXVECTOR3{ 0.0f, 0.0f, 0.0f };;
		}
	}

	D3DXVECTOR3 desiredDirection = target->GetPosition() - actor->position;
	D3DXVec3Normalize(&desiredDirection, &desiredDirection);
	D3DXVECTOR3 desiredVelocity =  desiredDirection * Game::getInstance()->blackboard->maxRedBallSpeed;
	D3DXVECTOR3 steering = desiredVelocity - actor->speed;

	energy -= D3DXVec3Length(&steering) * deltaTime;
    return steering;
}

void RedBallAI::findNewTargetBall()
{
    target = Game::getInstance()->blackboard->getNearestBall(actor->position);
}
