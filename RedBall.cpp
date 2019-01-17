#include "RedBall.h"

#include "CPR_Framework.h"
#include "Game.h"
#include "RedBallAI.h"
#include "Utils.h"
#include "YellowBall.h"

RedBall::RedBall(const D3DXVECTOR3& _position, const D3DXVECTOR3& _lookDir) : position{ _position }, speed{ _lookDir * 25.0f }
{
    ai = std::make_shared<RedBallAI>(this);
}

void RedBall::onRender()
{
    Render(Game::getInstance()->loader->GetMesh("unitsphere"), position, rotation, scale, color);
}

bool RedBall::onInit()
{
	return ai != nullptr;
}

void RedBall::onUpdate(float deltaTime)
{
    if (isDead()) {
        position.y = groundLevel;
        return;
    }

    std::shared_ptr<Building> building = Game::getInstance()->city->getBuildingActorIsColidingWith(position, 8.0f);
    if (building != nullptr) {
        speed = calculateReflectionSpeed(building);
	}
	else {
		speed += calculateNewSpeed(deltaTime);
	}
    position = position + speed * deltaTime;

    checkHitWithYellowBall();
}

void RedBall::checkHitWithYellowBall() const
{
    std::shared_ptr<YellowBall> nearestBall = Game::getInstance()->blackboard->getNearestBall(position);
    if (nearestBall != nullptr) {
        float distance = D3DXVec3Length(&(nearestBall->getPosition() - position));
        if (distance <= ballDiameter) {
            Game::getInstance()->blackboard->cleanUpAfterHit(nearestBall);
			ai->unsetTarget();
            ai->addEnergy(Game::getInstance()->blackboard->extraEnergyAfterHiting);
            ai->findNewTargetBall();
        }
    }
}

D3DXVECTOR3 RedBall::calculateNewSpeed(float deltaTime) const
{
	D3DXVECTOR3 force = ai->onUpdate(deltaTime);
	Utils::truncate(force, Game::getInstance()->blackboard->maxRedBallForce);

	const float speedValue = D3DXVec3Length(&speed);
	const D3DXVECTOR3 airResistance = -0.01f * speedValue * speed;

	return (force + airResistance + gravity) / weight * deltaTime;
}

D3DXVECTOR3 RedBall::calculateReflectionSpeed(const std::shared_ptr<Building>& building) const
{
	const D3DXVECTOR3 normal = building->getNormalAtPoint(position);
	const D3DXVECTOR3 vn = D3DXVec3Dot(&normal, &speed) * normal;
	const D3DXVECTOR3 vt = speed - vn;
	return vt - vn;
}
