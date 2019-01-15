#include "RedBallAI.h"
#include "Game.h"


RedBallAI::RedBallAI(RedBall* _actor) : actor(_actor)
{
    target = Game::GetInstance()->blackboard->getNearestBall(actor->position);
}


RedBallAI::~RedBallAI()
{
}


#include <sstream>
D3DXVECTOR3 RedBallAI::OnUpdate(float deltaTime)
{
    if (target == nullptr) { return D3DXVECTOR3{ 0.0f, 0.0f, 0.0f }; }
	if (energy < 0.0f) { return  D3DXVECTOR3{ 0.0f, 0.0f, 0.0f }; }

	D3DXVECTOR3 dir = target->GetPosition() - actor->position;
	D3DXVec3Normalize(&dir, &dir);
	D3DXVECTOR3 desired_velocity =  dir * Game::GetInstance()->blackboard->maxRedBallSpeed;
	D3DXVECTOR3 steering = desired_velocity - actor->speed;

	energy -= D3DXVec3Length(&steering) * deltaTime;
    return steering;
}

void RedBallAI::AddEnergy(float inc)
{
    energy += inc;
}

void RedBallAI::FindNewBall()
{
    target = Game::GetInstance()->blackboard->getNearestBall(actor->position);
}