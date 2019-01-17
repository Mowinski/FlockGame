#include "RedBall.h"
#include "Game.h"
#include "CPR_Framework.h"
#include "RedBallAI.h"
#include "Utils.h"
#include <limits>
#include <sstream>

RedBall::RedBall(D3DXVECTOR3 _position, D3DXVECTOR3 _lookDir) : position{ _position }, lookDir{ _lookDir }, speed{ _lookDir * 25.0f }
{
    ai = std::make_shared<RedBallAI>(this);
}


RedBall::~RedBall()
{
}

void RedBall::OnRender()
{
    Render(Game::GetInstance()->loader->GetMesh("unitsphere"), position, rotation, scale, color);
}

void RedBall::OnUpdate(float deltaTime)
{
    if (position.y <= groundLevel) {
        position.y = groundLevel;
        return;
    }

	D3DXVECTOR3 force = ai->OnUpdate(deltaTime);
	Utils::truncate(force, Game::GetInstance()->blackboard->maxRedBallForce);
	float speedValue = D3DXVec3Length(&speed);
    D3DXVECTOR3 airResistance = - 0.01f * speedValue * speed;

    speed += (force + airResistance + gravity) / weight * deltaTime;
    D3DXVECTOR3 newPosition{ position + speed * deltaTime };

    std::shared_ptr<Building> building = Game::GetInstance()->city->getBuildingActorIsColidingWith(newPosition, 8.0f);
    if (building != nullptr) {
        D3DXVECTOR3 normal = building->GetNormalAtPoint(newPosition);
        D3DXVECTOR3 vn = D3DXVec3Dot(&normal, &speed) * normal;
        D3DXVECTOR3 vt = speed - vn;
        speed = vt - vn;
        newPosition = position + speed * deltaTime;
    }
    position = newPosition;

    checkHitWithYellowBall();
}

void RedBall::checkHitWithYellowBall()
{
    std::shared_ptr<YellowBall> nearestBall = Game::GetInstance()->blackboard->getNearestBall(position);
    if (nearestBall != nullptr) {
        float distance = D3DXVec3Length(&(nearestBall->GetPosition() - position));
        if (distance <= 2 * ballSize) {
            Game::GetInstance()->blackboard->cleanUpAfterHit(nearestBall);
			unsetTarget();
            ai->AddEnergy(Game::GetInstance()->blackboard->extraEnergyAfterHiting);
            ai->FindNewTargetBall();
        }
    }
}

bool RedBall::OnInit()
{
    return true;
}

D3DXVECTOR3 RedBall::GetPosition() const
{
    return position;
}

std::shared_ptr<YellowBall> RedBall::getCurrentTarget() const
{
    return ai->target;
}

void RedBall::unsetTarget()
{
    ai->target = nullptr;
}

bool RedBall::isDead() const
{
    return position.y > groundLevel;
}
