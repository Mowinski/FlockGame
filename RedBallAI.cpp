#include "RedBallAI.h"
#include "Game.h"


RedBallAI::RedBallAI(RedBall* _actor) : actor(_actor)
{
	target = Game::GetInstance()->blackboard->getNearestBall(actor->position);
}


RedBallAI::~RedBallAI()
{
}

D3DXVECTOR3 RedBallAI::OnUpdate(float deltaTime)
{
	if (target == nullptr) { return D3DXVECTOR3{ 0.0f, 0.0f, 0.0f }; }
	D3DXVECTOR3 dir = target->GetPosition() - actor->position;
	float distance = D3DXVec3Length(&dir);
	if (distance < distanceToTarget) {
		distanceToTarget = distance;
		return D3DXVECTOR3{ 0.0f, 0.0f, 0.0f };
	}
	else if (energy > 0.0f) {
		energy -= 1.0f * deltaTime;
		return dir;
	}
	return D3DXVECTOR3{ 0.0f, 0.0f, 0.0f };
}

void RedBallAI::AddEnergy(float inc)
{
	energy += inc;
}
