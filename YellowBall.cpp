#include "YellowBall.h"

#include "CPR_Framework.h"
#include "Game.h"
#include "YellowBallAI.h"

YellowBall::YellowBall(std::shared_ptr<NavMeshItem> item)
{
    position = item->GetPosition();
    position.y = 2.0f;
    ai = std::make_shared<YellowBallAI>(this);
}

YellowBall::YellowBall(std::shared_ptr<NavMeshItem> item, float height)
{
    position = item->GetPosition();
    position.y = height;
    ai = std::make_shared<YellowBallAI>(this);
}

void YellowBall::onRender()
{
    Render(Game::getInstance()->loader->GetMesh("unitsphere"), position, rotation, scale, color);
}

void YellowBall::onUpdate(float deltaTime)
{
	D3DXVECTOR3 force = ai->onUpdate(deltaTime);
	speed += force / weight * deltaTime;
	position += speed * deltaTime;
}

bool YellowBall::onInit()
{
    return true;
}

std::shared_ptr<YellowBall> YellowBall::getNearestLeaderIfAny() const
{
    std::shared_ptr<YellowBall> ret = nullptr;

    float shortestDistance = visibilityDistance;

    for (auto leader : Game::getInstance()->blackboard->yellowBallsLeaders) {
        D3DXVECTOR3 diffVec = leader->getPosition() - getPosition();
        float distance = D3DXVec3Length(&diffVec);
        if (distance < shortestDistance) {
            ret = leader;
            shortestDistance = distance;
        }
    }
    return ret;
}

void YellowBall::setLeader()
{
    isLeader = true;
    ai->nominateToLeader();
}

void YellowBall::unsetLeader()
{
    isLeader = false;
	slotAquired.fill(nullptr);
}

bool YellowBall::checkTargetLeader(const std::shared_ptr<YellowBall>& ball) const
{
	return ai->targetLeader == ball;
}

void YellowBall::unsetTargetLeader()
{
	ai->targetLeader = nullptr;
	ai->state = YellowBallState::IDLE;
}

bool YellowBall::aquireSlot(int index, YellowBall* ball)
{
	if (slotAquired[index] == ball) {
		return true;
	}
	if (slotAquired[index] == nullptr) {
		slotAquired[index] = ball;
		return true;
	}
	return false;
}
