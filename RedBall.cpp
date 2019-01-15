#include "RedBall.h"
#include "Game.h"
#include "CPR_Framework.h"
#include "RedBallAI.h"
#include <limits>

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
    D3DXVECTOR3 speedNorm{};
    float speedValue = D3DXVec3Length(&speed);
    D3DXVec3Normalize(&speedNorm, &speed);
    D3DXVECTOR3 airResistance = - 0.0001f * speedValue * speedValue * speedNorm;

    speed += (force + airResistance + gravity) / weight * deltaTime;
    D3DXVECTOR3 newPosition{ position + speed * deltaTime };

    bool isCollideWithAnyBuilding = Game::GetInstance()->city->isCollideWithAnyBuilding(newPosition, 8.0f);
    if (isCollideWithAnyBuilding) {
        D3DXVECTOR3 normal = FindNormal(speedNorm);
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
            Game::GetInstance()->blackboard->destroyYellowBall(nearestBall);
            ai->AddEnergy(Game::GetInstance()->blackboard->extraEnergyAfterHiting);
            ai->FindNewBall();
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

D3DXVECTOR3 RedBall::FindNormal(const D3DXVECTOR3 & speedNorm) const
{
    /* @TODO Fix it! When angle < 45* */
    static D3DXVECTOR3 norms[] = {
        D3DXVECTOR3{1.0f, 0.0f, 0.0f},
        D3DXVECTOR3{0.0f, 0.0f, 1.0f},
        D3DXVECTOR3{-1.0f, 0.0f, 0.0f},
        D3DXVECTOR3{0.0f, 0.0f, -1.0f},
    };
    float minDist = (std::numeric_limits<float>::max)();
    int index = 0;

    for (int i = 0; i < sizeof(norms); ++i) {
        float dist = D3DXVec3Length(&(norms[i] - speedNorm));
        if (dist < minDist) {
            index = i;
            minDist = dist;
        }
    }
    return -norms[index];
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
