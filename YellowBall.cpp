#include "YellowBall.h"
#include "YellowBallAI.h"
#include "Game.h"
#include "Utils.h"


YellowBall::YellowBall(std::shared_ptr<NavMeshItem> item) : scale{ ballSize, ballSize, ballSize }
{
    position = item->position;
    position.y = 2.0f;
    ai = std::make_shared<YellowBallAI>(this, item);
}

YellowBall::YellowBall(std::shared_ptr<NavMeshItem> item, float height) : scale{ ballSize, ballSize, ballSize }
{
    position = item->position;
    position.y = height;
    ai = std::make_shared<YellowBallAI>(this, item);
}


YellowBall::~YellowBall()
{
}

void YellowBall::OnRender()
{
    Render(Game::GetInstance()->loader->GetMesh("unitsphere"), position, rotation, scale, color);
}

void YellowBall::OnUpdate(float deltaTime)
{
	D3DXVECTOR3 force = ai->OnUpdate(deltaTime);
	speed += force / weight * deltaTime;
	position += speed * deltaTime;
}

bool YellowBall::OnInit()
{
    return true;
}

D3DXVECTOR3 YellowBall::GetPosition() const
{
    return position;
}

D3DXVECTOR3 YellowBall::GetSpeed() const
{
    return speed;
}

std::shared_ptr<YellowBall> YellowBall::seeAnyLeader() const
{
    std::shared_ptr<YellowBall> ret = nullptr;

    if (D3DXVec3Length(&speed) == 0.0f) { return ret; }
    float shortestDistance = visibilityDistance;

    for (auto leader : Game::GetInstance()->blackboard->yellowBallsLeaders) {
        D3DXVECTOR3 diffVec = leader->GetPosition() - GetPosition();
        float distance = D3DXVec3Length(&diffVec);
        if (distance < shortestDistance) {
            ret = leader;
            shortestDistance = distance;
        }
    }
    return ret;
}

std::shared_ptr<NavMeshItem> YellowBall::GetCurrentNavMeshItem() const
{
    return Utils::getNearestNavMeshItem(position);
}

void YellowBall::SetLeader()
{
    isLeader = true;
    ai->nominateToLeader();
}

void YellowBall::UnsetLeader()
{
    isLeader = false;
	slotAquired.fill(nullptr);
}

bool YellowBall::checkTargetLeader(std::shared_ptr<YellowBall> ball) const
{
	return ai->targetLeader == ball;
}

void YellowBall::UnsetTargetLeader()
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
